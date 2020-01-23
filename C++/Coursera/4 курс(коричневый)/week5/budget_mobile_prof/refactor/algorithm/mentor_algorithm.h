#pragma once
#include <cstdint>
#include <cmath>
#include <memory>

/*
некий нетривиальный алгоритм предложенный менторами, без комментариев и названия (видимо дерево отрезков)
*/

struct IndexSegment
{
    size_t left;
    size_t right;

    size_t length() const;
    bool empty() const;
    bool Contains(IndexSegment other) const;
};

IndexSegment IntersectSegments(IndexSegment lhs, IndexSegment rhs);
bool AreSegmentsIntersected(IndexSegment lhs, IndexSegment rhs);

struct MoneyState
{
    double earned = 0.0;
    double spend = 0.0;

    double ComputeIncome() const;

    MoneyState& operator+=(const MoneyState& other);
    MoneyState operator+(const MoneyState& other) const;
    MoneyState operator*(double factor) const;
};

struct BulkMoneyAdder {
    MoneyState delta = {};
};

struct BulkTaxApplier {
    double factor;
    BulkTaxApplier(uint8_t percentage = 0) : factor(1.0 - percentage / 100.0) {}
};

class BulkLinearUpdater {
public:
    BulkLinearUpdater() = default;

    BulkLinearUpdater(const BulkTaxApplier& tax) : tax_(tax) {}
    BulkLinearUpdater(const BulkMoneyAdder& add): add_(add) {}

    void CombineWith(const BulkLinearUpdater& other);
    MoneyState Collapse(const MoneyState& origin, IndexSegment segment) const;
private:
    // apply tax first, then add
    BulkTaxApplier tax_;
    BulkMoneyAdder add_;
};


template <typename Data, typename BulkOperation>
class SummingSegmentTree {
public:
    SummingSegmentTree(size_t size) : root_(Build({ 0, size})) {}

    Data ComputeSum(IndexSegment segment) const {
        return this->TraverseWithQuery(root_, segment, ComputeSumVisitor{});
    }

    void AddBulkOperation(IndexSegment segment, const BulkOperation& operation) {
        this->TraverseWithQuery(root_, segment, AddBulkOperationVisitor{ operation });
    }

private:
    struct Node;
    using NodeHolder = std::unique_ptr<Node>;

    struct Node {
        NodeHolder left;
        NodeHolder right;
        IndexSegment segment;
        Data data;
        BulkOperation postponed_bulk_operation;
    };

    NodeHolder root_;

    static NodeHolder Build(IndexSegment segment) {
        if (segment.empty()) {
            return nullptr;
        }
        else if (segment.length() == 1) {
            return std::make_unique<Node>(Node{
              .left = nullptr,
              .right = nullptr,
              .segment = segment,
                });
        }
        else {
            const size_t middle = segment.left + segment.length() / 2;
            return std::make_unique<Node>(Node{
              .left = Build({segment.left, middle}),
              .right = Build({middle, segment.right}),
              .segment = segment,
                });
        }
    }

    template <typename Visitor>
    static typename Visitor::ResultType TraverseWithQuery(const NodeHolder& node, IndexSegment query_segment, Visitor visitor) {
        if (!node || !AreSegmentsIntersected(node->segment, query_segment)) {
            return visitor.ProcessEmpty(node);
        }
        else {
            PropagateBulkOperation(node);
            if (query_segment.Contains(node->segment)) {
                return visitor.ProcessFull(node);
            }
            else {
                if constexpr (is_void_v<typename Visitor::ResultType>) {
                    TraverseWithQuery(node->left, query_segment, visitor);
                    TraverseWithQuery(node->right, query_segment, visitor);
                    return visitor.ProcessPartial(node, query_segment);
                }
                else {
                    return visitor.ProcessPartial(
                        node, query_segment,
                        TraverseWithQuery(node->left, query_segment, visitor),
                        TraverseWithQuery(node->right, query_segment, visitor)
                    );
                }
            }
        }
    }

    class ComputeSumVisitor {
    public:
        using ResultType = Data;

        Data ProcessEmpty(const NodeHolder&) const {
            return {};
        }

        Data ProcessFull(const NodeHolder& node) const {
            return node->data;
        }

        Data ProcessPartial(const NodeHolder&, IndexSegment, const Data& left_result, const Data& right_result) const {
            return left_result + right_result;
        }
    };

    class AddBulkOperationVisitor {
    public:
        using ResultType = void;

        explicit AddBulkOperationVisitor(const BulkOperation& operation)
            : operation_(operation)
        {}

        void ProcessEmpty(const NodeHolder&) const {}

        void ProcessFull(const NodeHolder& node) const {
            node->postponed_bulk_operation.CombineWith(operation_);
            node->data = operation_.Collapse(node->data, node->segment);
        }

        void ProcessPartial(const NodeHolder& node, IndexSegment) const {
            node->data = (node->left ? node->left->data : Data()) + (node->right ? node->right->data : Data());
        }

    private:
        const BulkOperation& operation_;
    };

    static void PropagateBulkOperation(const NodeHolder& node) {
        for (auto* child_ptr : { node->left.get(), node->right.get() }) {
            if (child_ptr) {
                child_ptr->postponed_bulk_operation.CombineWith(node->postponed_bulk_operation);
                child_ptr->data = node->postponed_bulk_operation.Collapse(child_ptr->data, child_ptr->segment);
            }
        }
        node->postponed_bulk_operation = BulkOperation();
    }
};
