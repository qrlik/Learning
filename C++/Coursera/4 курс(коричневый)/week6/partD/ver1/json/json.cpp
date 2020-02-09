#include "json.h"
#include <sstream>

using namespace std;

namespace Json {

    Document::Document(Node root) : root(move(root)) {
    }

    const Node& Document::GetRoot() const {
        return root;
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

    Node LoadDouble(istream& input) {
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
        bool result = (input.get() == 't') ? true : false;
        input.ignore((result) ? 3 : 4);
        return result;
    }

    Node LoadNode(istream& input) {
        char c;
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
