def containers(types):
    return '\n\t'.join(
        list(map(lambda T: \
            f'std::vector<std::function<void(const {T["name"]}&)>> on{T["name"]};', types)))

def cases(types):
    return '\n\t\t\t'.join(list(map(lambda T: f'case {T["name"]}::type:\n\t\t\t\tdispatch(_STREAM, len - 1, on{T["name"]});\n\t\t\t\treturn;', types)))

def clientDispatcher(types):
    return '''
class Dispatcher {
public:
    <containers>
    void dispatch(std::istream& _STREAM, uint16_t len) {
        if (len == 0)
            throw std::exception("Invalid message, received length 0");
        uint8_t type;
        _STREAM.read((char*)&type, 1);
        switch (type) {
            <cases>
        }
    }
    template <typename T>
    void dispatch(std::istream& _STREAM, uint16_t len, const std::vector<std::function<void(const T&)>>& callbacks) {
        const T t(_STREAM, len);
        for (const auto& f : callbacks)
            f(t);
    }
};
''' .replace('<containers>', containers(types))\
    .replace('<cases>', cases(types))\
