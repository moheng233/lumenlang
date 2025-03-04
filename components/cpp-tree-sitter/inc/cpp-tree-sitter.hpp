#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

#include "tree_sitter/api.h"

namespace TreeSitter
{
    /**
     * An offset (usually in a UTF-8 string).
     */
    using Index = uint32_t;

    /**
     * A location in the source code.
     */
    using Point = TSPoint;

    /**
     * A range in the source code.
     */
    using Range = TSRange;

    /**
     * Represents edits made to a source code tree.
     */
    using Edit = TSInputEdit;

    /**
     * @brief Callback for editing/parsing source code.
     */
    using Input = std::function<
        std::string(
            Index startIndex,
            Point startPoint,
            Index endIndex)>;

    class Parser
    {
    public:
        /**
         * @brief Construct a new Parser object
         *
         * @param syntax Programming language to parse.
         */
        inline Parser(TSLanguage *langugae)
        {
            this->warp = ts_parser_new();
            ts_parser_set_language(this->warp, langugae);
        }
        /** Destructor. */
        inline ~Parser()
        {
            ts_parser_delete(this->warp);
        }

        /** Parse source code into an AST. */
        Tree parse(const std::string &input)
        {
            ts_parser_parse_string(this->warp, NULL, input.c_str(), input.length());
        }

        /** Parse source code into an AST. */
        Tree parse(Tree oldTree, const std::string &input)
        {
            ts_parser_parse_string(this->warp, oldTree.original(), input.c_str(), input.length());
        }

        /** Reset the internal state. */
        void reset()
        {
            ts_parser_reset(this->warp);
        }

        /**
         * @brief Get the timeout in milliseconds.
         *
         * @return uint64_t Timeout length.
         */
        uint64_t timeout() const
        {
            return ts_parser_timeout_micros(this->warp);
        }

        /**
         * @brief Set the timeout in milliseconds.
         *
         * @param value Timeout length.
         */
        void setTimeout(uint64_t value)
        {
            ts_parser_set_timeout_micros(this->warp, value);
        }

    private:
        TSParser *warp;
    };

    class Node
    {
    private:
        TSNode warp;

    public:
        Node(TSNode node)
        {
            this->warp = node;
        }

        TSNode original()
        {
            return this->warp;
        }

        Node field(std::string &name)
        {
            return Node(
                ts_node_child_by_field_name(this->warp, name.c_str(), name.length()));
        }

        const char *name()
        {
            return ts_node_type(this->warp);
        }

        const bool named()
        {
            return ts_node_is_named(this->warp);
        }
    };

    class Cursor
    {
    public:
        Cursor(TSTreeCursor cursor)
        {
            this->warp = cursor;
        }
        ~Cursor()
        {
            ts_tree_cursor_delete(&this->warp);
        }

        void reset(Node node)
        {
            ts_tree_cursor_reset(&this->warp, node.original());
        }

    private:
        TSTreeCursor warp;
    };

    class Tree
    {
    public:
        /** @internal Copy constructor. */
        Tree(TSTree *tree)
        {
            this->warp = tree;
        }
        /** @internal Destructor. */
        ~Tree()
        {
            ts_tree_delete(this->warp);
        }

        /** The highest level node of this tree. */
        Node rootNode() const
        {
            return Node(ts_tree_root_node(this->warp));
        }
        /** The programming language used by this tree. */
        const TSLanguage *language() const
        {
            return ts_tree_language(this->warp);
        }

        /** Create a copy of this tree. */
        Tree *copy()
        {
            return new Tree(ts_tree_copy(this->warp));
        }

        /** @private Only used by Parser. */
        TSTree *original() const
        {
            return this->warp;
        }

        /** Add an edit to this tree. */
        void edit(const Edit *delta)
        {
            ts_tree_edit(this->warp, delta);
        }

        /** Construct a walker to navigate this tree. */
        Cursor walk(Node node)
        {
            return Cursor(ts_tree_cursor_new(node.original()));
        }

        /** A list of changed areas. */
        std::vector<Range> getChangedRanges(Tree other)
        {
            uint32_t length;

            TSRange *warps = ts_tree_get_changed_ranges(this->warp, other.original(), &length);

            std::vector<Range> ranges(length);
            for (size_t i = 0; i < length; i++)
            {
                ranges[i] = warps[i];
            }

            free(warps);
        }

    private:
        TSTree *warp;
    };
}
