#include "ReleaseNotesDialog.h"
#include "ui_ReleaseNotesDialog.h"

#include <md4c-html.h>

static QString fixupHtmlEmojiBug(const QString & html)
{
    // For some reason surrogates do not always display correctly in HTML elements.
    // As a workaround we add a zero-width space in front of the high surrogate.
    QString result;
    int size = html.size();
    result.reserve(size);
    for(int i = 0; i < size; i++)
    {
        auto ch = html[i];
        if(ch.isHighSurrogate() && i + 1 < size)
        {
            result += QChar(0x200B);
        }
        result += ch;
    }
    return result;
}

static QString markdownToHtml(const QByteArray & markdown)
{
    auto appendString = [](const MD_CHAR * text, MD_SIZE size, void* userdata)
    {
        ((std::string*)userdata)->append(text, size);
    };
    std::string html;
    unsigned int parserFlags =
        MD_FLAG_COLLAPSEWHITESPACE |
        MD_FLAG_TABLES |
        MD_FLAG_STRIKETHROUGH |
        MD_FLAG_TASKLISTS |
        MD_FLAG_PERMISSIVEAUTOLINKS |
        MD_FLAG_LATEXMATHSPANS;
    unsigned int rendererFlags = 0;
    auto result = md_html(markdown.constData(), markdown.size(), appendString, &html, parserFlags, rendererFlags);
    if(result != 0)
    {
        // TODO: throw an exception instead?
        return {};
    }
    return QString::fromStdString(html);
}

ReleaseNotesDialog::ReleaseNotesDialog(const QByteArray & markdown, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::ReleaseNotesDialog)
{
    ui->setupUi(this);

    auto font = QApplication::font();
    font.setPointSize(16);
    ui->textBrowser->document()->setDefaultFont(font);

    ui->textBrowser->setOpenExternalLinks(true);

    auto html = markdownToHtml(markdown);
    html = fixupHtmlEmojiBug(html);
    ui->textBrowser->setText(html);
}

ReleaseNotesDialog::~ReleaseNotesDialog()
{
    delete ui;
}
