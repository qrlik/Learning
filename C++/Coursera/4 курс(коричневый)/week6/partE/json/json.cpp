#include "json.h"
#include <sstream>

using namespace std;

namespace Json {
    /* Node realization */
    namespace {
        string fixDouble(double d) {
            ostringstream os;
            os.setf(ios::fixed);
            os.precision(6);
            os << d;

            string str = os.str();
            size_t end = str.find_last_not_of('0') + 1;
            /* npos не будет, т.к всегда есть '.' (0.000000) */
            if (end <= str.size()) {
                str.erase(end);
            }
            if (str.back() == '.') {
                str.pop_back();
            }
            return str;
        }

        ostream& operator<<(ostream& os, const map<string, Node>& map) {
            os << '{';
            bool first = true;
            for (const auto& [key, value] : map) {
                if (!first) {
                    os << ",";
                }
                else {
                    first = false;
                }

                os << '\n' << '"' << key << "\": " << value;
            }
            return os << "\n}";
        }

        ostream& operator<<(ostream& os, const vector<Node>& vector) {
            os << '[';
            bool first = true;
            for (const auto& node : vector) {
                if (!first) {
                    os << ",";
                }
                else {
                    first = false;
                }

                os << '\n' << node;
            }
            return os << "\n]";
        }
    }

    size_t Node::GetIndex() const {
        return index();
    }

    ostream& operator<<(ostream& os, const Node& node) {
        switch (node.GetIndex())
        {
        case(0):
            os << node.AsMap();
            break;
        case(1):
            os << node.AsArray();
            break;
        case(2):
            os << '"' << node.AsString() << '"';
            break;
        case(3):
            os << fixDouble(node.AsDouble());
            break;
        default: //4
            os.setf(ios::boolalpha);
            os << node.AsBool();
            break;
        }
        return os;
    }

    /* Document realization */
    Document::Document(Node root) : root(move(root)) {
    }

    const Node& Document::GetRoot() const {
        return root;
    }

    ostream& operator<<(ostream& os, const Document& doc) {
        return os << doc.GetRoot();
    }

    Node LoadNode(istream& input);

    Node LoadArray(istream& input) {
        vector<Node> result;

        for (char c; input >> c && c != ']'; ) {
            if (c != ',') {
                input.putback(c);
            }
            result.push_back(LoadNode(input));
        }

        return Node(move(result));
    }

    namespace {
        void GetDigits(string& str, istream& input) {
            char c = static_cast<char>(input.get());
            for (; isdigit(c); c = static_cast<char>(input.get())) {
                str.push_back(c);
            }
            input.putback(c);
        }
    }

    Node LoadDouble(std::istream& input) {
        string str_double;
        if (input.peek() == '-') {
            str_double.push_back(static_cast<char>(input.get()));
        }

        GetDigits(str_double, input);
        if (input.peek() == '.') {
            str_double.push_back(static_cast<char>(input.get()));
            GetDigits(str_double, input);
        }

        size_t pos;
        double result = stod(str_double, &pos);

        if (pos != str_double.length()) {
            ostringstream error;
            error << "string " << str_double << " contains " << (str_double.length() - pos) << " trailing chars";
            throw invalid_argument(error.str());
        }

        return Node(result);
    }

    Node LoadString(istream& input) {
        string line;
        getline(input, line, '"');
        return Node(move(line));
    }

    Node LoadDict(istream& input) {
        map<string, Node> result;

        for (char c; input >> c && c != '}'; ) {
            if (c == ',') {
                input >> c;
            }

            string key = LoadString(input).AsString();
            input >> c;
            result.emplace(move(key), LoadNode(input));
        }

        return Node(move(result));
    }

    Node LoadBool(istream& input) {
        string bool_word;
        bool_word.resize(4);
        input.read(bool_word.data(), 4);
        if (bool_word == "true") {
            return true;
        }
        else if (bool_word == "fals") {
            if (char c = static_cast<char>(input.get());
                c == 'e') {
                return false;
            }
            else {
                input.putback(c);
            }
        }

        for (auto i = bool_word.rbegin(); i != bool_word.rend(); ++i) {
            input.putback(*i);
        }

        throw invalid_argument("Wrong bool word in istream -> " + move(bool_word));
    }

    Node LoadNode(istream& input) {
        char c = 0;
        input >> c;

        if (c == '[') {
            return LoadArray(input);
        }
        else if (c == '{') {
            return LoadDict(input);
        }
        else if (c == '"') {
            return LoadString(input);
        }
        else if (isdigit(c) || c == '-') {
            input.putback(c);
            return LoadDouble(input);
        }
        else {
            input.putback(c);
            return LoadBool(input);
        }
    }

    Document Load(istream& input) {
        return Document{ LoadNode(input) };
    }
}

