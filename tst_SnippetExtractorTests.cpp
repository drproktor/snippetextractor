#include <QString>
#include <QByteArray>

#include <QtTest>

#include "Exception.h"
#include "Helpers.h"
#include "Process.h"

class SnippetExtractorTests : public QObject
{
    Q_OBJECT

public:
    SnippetExtractorTests();
private:
    QByteArray loadResource(const QString& path);

private Q_SLOTS:
    void testSnippetSample();
    void testSnippetSampleMinted();
    void testSnippetSampleNested();
    void testIncludeSample();
    void testCMakeFormatIncludeSample();
    void testErrorOnMissingSnippet();
};

SnippetExtractorTests::SnippetExtractorTests()
{
}

QByteArray SnippetExtractorTests::loadResource(const QString &path)
{
    QFile input(path);
    if (!input.open(QIODevice::ReadOnly)) {
        Q_ASSERT_X(false, Q_FUNC_INFO, "resource cannot be opened");
    }
    return input.readAll();
}

void SnippetExtractorTests::testIncludeSample()
{
    try {
        const QString result = process(":/data/testresources/include-sample.md.in");
        const QString expected = loadResource(":/data/testresources/include-sample.md");
        QCOMPARE(result, expected);
    } catch (const Exception& ex) {
        QFAIL(ex.message().toLocal8Bit());
    }
}

void SnippetExtractorTests::testCMakeFormatIncludeSample()
{
    try {
        const QString result = process(":/data/testresources/include-sample-cmake.md.in");
        const QString expected = loadResource(":/data/testresources/include-sample-cmake.md");
        QCOMPARE(result, expected);
    } catch (const Exception& ex) {
        QFAIL(ex.message().toLocal8Bit());
    }
}

void SnippetExtractorTests::testErrorOnMissingSnippet()
{
    try {
        const QString result = process(":/data/testresources/missing-sample.md.in");
        QFAIL("Missing samples should result in errors!");
    } catch (const Exception&) {
        //all good
    }
}

void SnippetExtractorTests::testSnippetSample()
{
  try {
    const QString args = QString("%1,%2,%3").arg(":/data/testresources/sample.cpp")
      .arg("sample").arg("cpp");
    const QString pos;
    const QString result = process_snippet(args,pos);
    const QString expected = loadResource(":/data/testresources/snippet-sample.md");
    QCOMPARE(result, expected);
  } catch (const Exception& ex) {
    QFAIL(ex.message().toLocal8Bit());
  }
}


void SnippetExtractorTests::testSnippetSampleMinted()
{
  try {
    const QString args = QString("%1,%2,%3,%4")
      .arg(":/data/testresources/sample.cpp")
      .arg("sample").arg("c++").arg("minted");
    const QString pos;
    const QString result = process_snippet(args,pos);
    const QString expected = loadResource(":/data/testresources/snippet-sample-minted.tex");
    QCOMPARE(result, expected);
  } catch (const Exception& ex) {
    QFAIL(ex.message().toLocal8Bit());
  }
}

void SnippetExtractorTests::testSnippetSampleNested()
{
  try {
    QString args = QString("%1,%2,%3,%4")
      .arg(":/data/testresources/sample-nested.cpp")
      .arg("sample-nested-1").arg("c++").arg("minted");
    QString pos;
    QString result = process_snippet(args,pos);
    QString expected = loadResource(":/data/testresources/snippet-sample-nested-1-minted.tex");
    QCOMPARE(result, expected);
    args = QString("%1,%2,%3,%4")
      .arg(":/data/testresources/sample-nested.cpp")
      .arg("sample-nested-2").arg("c++").arg("minted");
    result = process_snippet(args,pos);
    expected = loadResource(":/data/testresources/snippet-sample-nested-2-minted.tex");
    QCOMPARE(result, expected);
    args = QString("%1,%2,%3,%4")
      .arg(":/data/testresources/sample-nested.cpp")
      .arg("sample-nested-3").arg("c++").arg("minted");
    result = process_snippet(args,pos);
    expected = loadResource(":/data/testresources/snippet-sample-nested-3-minted.tex");
    QCOMPARE(result, expected);
   } catch (const Exception& ex) {
    QFAIL(ex.message().toLocal8Bit());
  }
}


QTEST_APPLESS_MAIN(SnippetExtractorTests)

#include "tst_SnippetExtractorTests.moc"
