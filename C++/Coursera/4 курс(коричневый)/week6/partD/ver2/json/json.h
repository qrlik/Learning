#pragma once

#include <istream>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace Json {
    class Node : std::variant<std::map<std::string, Node>,
        std::vector<Node>, std::string,
        double, bool> {
    public:
        using variant::variant;

        const auto& AsMap() const {
            return std::get<std::map<std::string, Node>>(*this);
        }
        const auto& AsArray() const {
            return std::get<std::vector<Node>>(*this);
        }
        const auto& AsString() const {
            return std::get<std::string>(*this);
        }
        double AsDouble() const {
            return std::get<double>(*this);
        }
        bool AsBool() const {
            return std::get<bool>(*this);
        }

        size_t GetIndex() const;
    };

    std::ostream& operator<<(std::ostream& os, const Node& doc);

    class Document {
        Node root;
    public:
        explicit Document(Node root);

        const Node& GetRoot() const;
    };

    std::ostream& operator<<(std::ostream& os, const Document& doc);
    Document Load(std::istream& input);
}
