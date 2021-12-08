from constants import intTypes, primitives

def isSimpleTypeField(field):
    fields = field['fields']
    return len(fields) == 1 and not 'fields' in fields[0]

def isIntField(simpleTypeField):
    return simpleTypeField['fields'][0]['name'] in intTypes

def isStringField(simpleTypeField):
    return simpleTypeField['fields'][0]['name'] == 'str'

M = {
    'i8' : 'int8_t'  ,
    'u8' : 'uint8_t' ,
    'i16': 'int16_t' ,
    'u16': 'uint16_t',
    'i32': 'int32_t' ,
    'u32': 'uint32_t',
    'str': 'std::string',
}

def primitiveTypeToCPPType(T):
    if T in M:
        return M[T]
    elif not '[]' in T:
        return T
    elif 'str[]' in T:
        return 'std::vector<std::string>'
    else:
        return f'std::vector<{T.replace("[]", "")}>'

def simpleTypeFieldToTypeAndName(field):
    return primitiveTypeToCPPType(field['fields'][0]['name']) + ' ' + field['name']

def simpleTypeFieldToTypeRHRAndName(field):
    return  primitiveTypeToCPPType(field['fields'][0]['name']) + '&& ' + ' ' + field['name']

def simpleTypeFieldToDeclaration(field):
    return  simpleTypeFieldToTypeAndName(field) + ';\n'

def declareTFields(T_Fields):
    return '\t'.join(list(map(simpleTypeFieldToDeclaration, T_Fields)))

def declareFieldLength(field):
    return f'uint8_t mutable _length_{field["name"]};\n'

def declareFieldLengths(T_Fields):
    return '\t'.join(list(map(declareFieldLength, T_Fields)))

def constructorParams(intFields, stringFields, customFields):
    return ','.join(list(map(simpleTypeFieldToTypeAndName, intFields)) + list(map(simpleTypeFieldToTypeRHRAndName, stringFields + customFields)))

def initializer(field):
    name = field['name']
    return f'{name}({name})'

def moveInitializer(field):
    name = field['name']
    return f'{name}(std::move({name}))'

def lenInitializer(strField):
    return f'_length_{strField["name"]}({strField["name"]}.length())'

def initializers(intFields, stringFields, customFields):
    if len(intFields) == 0 and len(stringFields) == 0 and len(customFields) == 0:
        return ' = default;'
    return ' : ' + ','.join(
        list(map(initializer, intFields)) +
        list(map(moveInitializer, stringFields + customFields)) +
        list(map(lenInitializer, stringFields))) + ' {}'

def bytesPerType(T):
    if '8' in T:
        return 1
    elif '16' in T:
        return 2
    elif '32' in T:
        return 4

def mutateLengths(strFields, lenMethod):
    return '\n'.join(list(map(lambda field: f'_length_{field["name"]} = {field["name"]}.{lenMethod}();', strFields)))

def writeIntFields(intFields, stringFields, strVectorFields, customVectorFields):
    size = sum([bytesPerType(f['fields'][0]['name']) for f in intFields]) + len(stringFields + strVectorFields + customVectorFields)
    return  mutateLengths(stringFields, 'length') + '\n\t\t' + \
            mutateLengths(strVectorFields, 'size') + '\n\t\t' + \
            mutateLengths(customVectorFields, 'size') + '\n\t\t'+ \
            f'_STREAM.write((char*)this, {size});'

def readIntFields(intFields, stringFields, strVectorFields, customVectorFields):
    size = sum([bytesPerType(f['fields'][0]['name']) for f in intFields]) + len(stringFields + strVectorFields + customVectorFields)
    return  f'if (_STREAM.tellg().operator+({size}) > _STREAM_LENGTH + _tellg_beg)' + " {\n\t\t\t" + \
            f'std::string ERRMSG = std::to_string(_STREAM.tellg()) + \",\" + std::to_string({size})+ \",\" + std::to_string(_STREAM_LENGTH);\n\t\t\t' + \
             'throw std::out_of_range(ERRMSG);\n\t\t}\n\t\t' + \
            f'_STREAM.read((char*)this, {size});'

def readStrField(strField):
    return "{\n\t\t\t" + \
    f"uint8_t L = _length_{strField['name']};\n\t\t\t" + \
    f"if (_STREAM.tellg().operator+(L) > _STREAM_LENGTH + _tellg_beg)" + " {\n\t\t\t" + \
    "std::string ERRMSG = std::to_string(_STREAM.tellg()) + \",\" + std::to_string(L)+ \",\" + std::to_string(_STREAM_LENGTH);\n\t\t\t" + \
    "throw std::out_of_range(ERRMSG);\n\t\t\t" + \
    "}\n\t\t\t" + \
    f"char* temp = new char[L + 1];\n\t\t\t" + \
    f"_STREAM.read(temp, L);\n\t\t\t" + \
    f"temp[L] = '\\0';\n\t\t\t" + \
    f"{strField['name']} = temp;\n\t\t" + \
    f"delete[] temp;\n\t\t" + \
    "}\n"

def readStrFields(stringFields):
    return ''.join(list(map(readStrField, stringFields)))

def readCustomField(customField):
    return f'{customField["name"]} = std::move({customField["fields"][0]["name"]}(_STREAM, _STREAM_LENGTH));'

def readCustomFields(stringFields):
    return ''.join(list(map(readCustomField, stringFields)))

def writeStrField(strField):
    return f"_STREAM.write({strField['name']}.c_str(), _length_{strField['name']});"

def writeStrFields(stringFields):
    return ''.join(list(map(writeStrField, stringFields)))

def writeCustomField(customField):
    return f'{customField["name"]}.write<false>(_STREAM);'

def writeCustomFields(customFields):
    return '\n'.join(list(map(writeCustomField, customFields)))

def readStrVecField(strVecField):
    return """
        <NAME>.reserve(_length_<NAME>);
        for (int i = 0; i < _length_<NAME>; ++i) {
            uint8_t LEN;
            if (_STREAM.tellg().operator+(1) > _STREAM_LENGTH + _tellg_beg) {
                std::string ERRMSG = std::to_string(_STREAM.tellg()) + \",1,\" + std::to_string(_STREAM_LENGTH);
                throw std::out_of_range(ERRMSG);
            }
            _STREAM.read((char*)&LEN, 1);
            if (_STREAM.tellg().operator+(LEN) > _STREAM_LENGTH + _tellg_beg) {
                std::string ERRMSG = std::to_string(_STREAM.tellg()) + \",\" + std::to_string(LEN)+ \",\" + std::to_string(_STREAM_LENGTH);
                throw std::out_of_range(ERRMSG);
            }
            char* temp = new char[LEN + 1];
            _STREAM.read(temp, LEN);
            temp[LEN] = '\\0';
            <NAME>.emplace_back(temp);
            delete[] temp;
        }
    """.replace("<NAME>", strVecField['name'])

def readStrVecFields(strVecFields):
    return '\n'.join(list(map(readStrVecField, strVecFields)))

def readCustomVecField(customVecField):
    return """
        <NAME>.reserve(_length_<NAME>);
        for (uint8_t i = 0; i < _length_<NAME>; ++i)
            <NAME>.emplace_back(std::move(<TYPE>(_STREAM, _STREAM_LENGTH)));
    """ .replace("<NAME>", customVecField['name'])\
        .replace("<TYPE>", customVecField['fields'][0]['name'].replace('[]', ''))

def readCustomVecFields(customVecFields):
    return '\n'.join(list(map(readCustomVecField, customVecFields)))

def writeStrVecField(strVecField):
    return """
        for (const auto& f : <FIELD>) {
            uint8_t len = f.length();
            _STREAM.write((char*)&len, sizeof(len));
            _STREAM.write(f.c_str(), len);
        }
    """.replace('<FIELD>', strVecField['name'])

def writeStrVecFields(strVecFields):
    return '\n'.join(list(map(writeStrVecField, strVecFields)))

def writeCustomVecField(customVecField):
    return """
        for (const auto& f : <FIELD>)
            f.write<false>(_STREAM);
    """.replace('<FIELD>', customVecField['name'])

def writeCustomVecFields(customVecFields):
    return '\n'.join(list(map(writeCustomVecField, customVecFields)))

def makeClass(T, readableFromStream, writableToStream):
    index = T['index']
    intFields          = list(filter(isIntField       , T['fields']))
    stringFields       = list(filter(isStringField    , T['fields']))
    customFields       = list(filter(lambda f: not isIntField(f) and not isStringField(f) and not '[]' in f['fields'][0]['name'], T['fields']))
    strVectorFields    = list(filter(lambda f:                                                 'str[]' in f['fields'][0]['name'], T['fields']))
    customVectorFields = list(filter(lambda f:         '[]' in f['fields'][0]['name'] and  not 'str[]' in f['fields'][0]['name'], T['fields']))
    return '\n' +'\n'.join([l for l in '''
class <name> {
public:
    constexpr static uint8_t type = <index>;
    <declare int fields>
    <declare str fields>
    <declare str[] fields>
    <declare custom fields>
    <declare custom[] fields>
    <empty ctor if needed>
    <name>(<constructor params>) <initializers and body or default>
    <from stream>
    <to stream>
};
''' .replace('<from stream>', '' if not readableFromStream else '''
    <name>(std::istream& _STREAM, const int _STREAM_LENGTH) {
        uint16_t _tellg_beg = _STREAM.tellg();
        <read int fields>
        <read str fields>
        <read str[] fields>
        <read custom fields>
        <read custom[] fields>
    }''')
    .replace('<to stream>', '' if not writableToStream else '''
    template <bool writeType>
    uint16_t write(std::ostream& _STREAM) const {
        uint16_t _tellp_beg;
        if constexpr (writeType) {
            _tellp_beg = _STREAM.tellp();
            _STREAM.seekp(2); // 2 bytes for size
            _STREAM.write((char*)&type, 1);
        }
        <write int fields>
        <write str fields>
        <write str[] fields>
        <write custom fields>
        <write custom[] fields>
        if constexpr (writeType) {
            uint16_t size = _STREAM.tellp().operator-(2 + _tellp_beg); // remove 2 bytes storing the size itself
            _STREAM.seekp(_tellp_beg);
            _STREAM.write((char*)&size, 2);
            return size + 2; // total length includes 2 bytes for size
        }
        return -1;
    }''')
    .replace('<empty ctor if needed>', '<name>() = default;' if len(intFields) != 0 or len(stringFields) != 0 or len(customFields) != 0 else '')\
    .replace('<name>', T['name'])\
    .replace('<index>', str(index))\
    .replace('<declare int fields>', declareTFields(intFields) + '\t' + declareFieldLengths(strVectorFields + customVectorFields + stringFields))\
    .replace('<declare str fields>', declareTFields(stringFields))\
    .replace('<declare str[] fields>', declareTFields(strVectorFields))\
    .replace('<declare custom fields>', declareTFields(customFields))\
    .replace('<declare custom[] fields>', declareTFields(customVectorFields))\
    .replace('<constructor params>', constructorParams(intFields, stringFields, customFields))\
    .replace('<initializers and body or default>', initializers(intFields, stringFields, customFields))\
    .replace('<read int fields>', readIntFields(intFields, stringFields, strVectorFields, customVectorFields))\
    .replace('<read str fields>', readStrFields(stringFields))\
    .replace('<read str[] fields>', readStrVecFields(strVectorFields))\
    .replace('<read custom fields>', readCustomFields(customFields))\
    .replace('<read custom[] fields>', readCustomVecFields(customVectorFields))\
    .replace('<write int fields>', writeIntFields(intFields, stringFields, strVectorFields, customVectorFields))\
    .replace('<write str fields>', writeStrFields(stringFields))\
    .replace('<write str[] fields>', writeStrVecFields(strVectorFields))\
    .replace('<write custom fields>', writeCustomFields(customFields))\
    .replace('<write custom[] fields>', writeCustomVecFields(customVectorFields))\
    .split('\n') if len(l.strip())])
