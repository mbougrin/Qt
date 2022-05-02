#include "mdrawtext.h"

MDTitle::MDTitle(const std::string &text)
{
    this->m_text = text;
}

void MDTitle::parseHtml(const std::string &text)
{
    std::string str = "";

    for (size_t i = 0 ; i < text.size() ; ++i) {
        int markdown = 0;
        if (text.at(i) == '#') {
            while (text.at(i) == '#') {
                i++;
                markdown++;
            }
            str.append("<h" + std::to_string(markdown) + ">");
            while (text.at(i) != '\n') {
                if (text.at(i) != ' ')
                    str += m_text.at(i);
                i++;
            }
            str.append("</h" + std::to_string(markdown) + ">");
            str += text.at(i);
        } else {
            str.append("<p>");
            while (i < text.size()) {
                if (text.at(i) != ' ' && text.at(i) != '\n')
                    str += text.at(i);
                i++;
            }
            str.append("<p>");
        }
    }
    m_html = str;
}

void MDTitle::parseText(const std::string &text)
{
    std::string str = "";

    for (size_t i = 0 ; i < m_text.size() ; ++i) {
        if (text.at(i) != ' ')
           str += m_text.at(i);
    }
    m_text = str;
}

std::shared_ptr<MDContent> MDTitle::parseMD(const std::string & text)
{
    m_content = new MDContent;
    std::shared_ptr<MDContent> md_content(m_content);
    parseHtml(text);
    parseText(text);
    std::shared_ptr<MDTitle> md_raw_text(this);
    md_content->addItem(md_raw_text);
    return md_content;
}

std::string MDTitle::toString() const
{
    return m_text;
}

std::string MDTitle::toHTML() const
{
    return m_html;
}

MDRawText::MDRawText(const std::string & text)
{
    this->m_text = text;
}

std::string MDRawText::toString() const
{
    return m_text;
}

std::string MDRawText::toHTML() const
{
    return m_text;
}
