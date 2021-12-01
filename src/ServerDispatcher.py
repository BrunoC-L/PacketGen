def containers(types):
    return '\n\t'.join(\
        list(map(lambda T: \
            f'std::vector<std::function<void(Sender& sender, const {T["name"]}&)>> on{T["name"]};', types)))

def cases(types):
    return '\n\t\t\t'.join(list(map(lambda T: f'case {T["name"]}::type:\n\t\t\t\tdispatch(_STREAM, len - 1, sender, on{T["name"]});\n\t\t\t\treturn;', types)))

def serverDispatcher(types):
    return '''
template <class Sender>
class Dispatcher {
public:
    <containers>
    void dispatch(std::istream& _STREAM, uint16_t len, Sender& sender) {
        if (len == 0)
            throw std::exception("Invalid message, received length 0");
        uint8_t type;
        _STREAM.read((char*)&type, 1);
        switch (type) {
            <cases>
        }
    }
    template <typename T>
    void dispatch(std::istream& _STREAM, uint16_t len, Sender& sender, const std::vector<std::function<void(Sender& sender, const T&)>>& callbacks) {
        T t(_STREAM, len);
        for (const auto& f : callbacks)
            f(sender, t);
    }
};
''' .replace('<containers>', containers(types))\
    .replace('<cases>', cases(types))\
