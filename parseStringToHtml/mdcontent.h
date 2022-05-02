#ifndef MD_TOOLS
#define MD_TOOLS

#include <memory>
#include <string>
#include <vector>

class MDItem {
public:
    virtual std::string toString() const = 0;
    virtual std::string toHTML() const = 0;
};

class MDContent : public MDItem {
private:
    std::vector<std::shared_ptr<MDItem>> m_items;
public:
    MDContent() {}
    void addItem(std::shared_ptr<MDItem> it) {
        m_items.push_back(it);
    }
    std::string toString() const {
        std::string md;
        for(const auto & item: m_items) {
            md += item->toString();
        }
        return md;
    }
    std::string toHTML() const {
        std::string html;
        for(const auto & item: m_items) {
            html += item->toHTML();
        }
        return html;
    }
};

#endif // MD_TOOLS
