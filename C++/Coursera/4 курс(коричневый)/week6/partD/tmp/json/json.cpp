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
            while (isdigit(input.peek())) {
                str.push_back(static_cast<char>(input.get()));
            }
        }
    }

    Node LoadDouble(istream& input) {
        string str_double;

        GetDigits(str_double, input);
        if (input.peek() == '.') {
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
        if (input.get() == 't' && input.get() == 'r' &&
            input.get() == 'u' && input.get() == 'e') {
            return Node(true);
        }
        return Node(false);
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
        else if (isdigit(c)) {
            input.putback(c);
            return LoadDouble(input);
        }
        else {
            return LoadBool(input);
        }
    }

    Document Load(istream& input) {
        return Document{ LoadNode(input) };
    }

}
