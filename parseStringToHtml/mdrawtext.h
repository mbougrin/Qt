#ifndef MDRAWTEXT_H
#define MDRAWTEXT_H

#include <mdcontent.h>

class MDRawText : public MDItem {
private:
    std::string m_text;
public:
    MDRawText(const std::string &text);
    std::string toString() const;
    std::string toHTML() const;
};

class MDTitle : public MDItem {
private:
    void            parseText(const std::string &text);
    void            parseHtml(const std::string &text);
    std::string     m_text;
    std::string     m_html;
    MDContent       *m_content;
public:
    std::shared_ptr<MDContent> parseMD(const std::string & text);
    MDTitle(const std::string &text);
    std::string toString() const;
    std::string toHTML() const;
};

#endif // MDRAWTEXT_H
