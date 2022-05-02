#include <QCoreApplication>
#include <mdrawtext.h>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::shared_ptr<MDRawText> md_raw_text_1(new MDRawText("bienvenue "));
    std::shared_ptr<MDRawText> md_raw_text_2(new MDRawText("chez "));
    std::shared_ptr<MDRawText> md_raw_text_3(new MDRawText("ipso !"));
    std::shared_ptr<MDContent> md_content(new MDContent);
    md_content->addItem(md_raw_text_1);
    md_content->addItem(md_raw_text_2);
    md_content->addItem(md_raw_text_3);
    std::cout << "markdown: " << md_content->toString() << std::endl;
    std::cout << "html: " << md_content->toHTML() << std::endl;

    std::string txt = "# Title\n## Subtitle\nContent";
    MDTitle *title = new MDTitle(txt);
    std::shared_ptr<MDContent> content = title->parseMD(txt);
    std::cout << content->toString() << std::endl;
    std::cout << content->toHTML() << std::endl;
    return 0;
}
