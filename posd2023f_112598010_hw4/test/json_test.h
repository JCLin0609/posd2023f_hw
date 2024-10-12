#include "../src/json_object.h"
#include "../src/string_value.h"
#include "../src/visitor.h"
#include "../src/beautify_visitor.h"
#include "../src/json_builder.h"
#include "../src/json_scanner.h"
#include "../src/json_parser.h"

TEST(JSonSuite, OneKeyStringValue) {
    JsonObject *jo = new JsonObject;
    Value * v1 = new StringValue("value1");
    jo->set("key1", v1);
    ASSERT_EQ(v1, jo->getValue("key1"));
    ASSERT_EQ("{\n\"key1\":\"value1\"\n}", jo->toString());

    delete jo;
    delete v1;
}

TEST(JSonSuite, TwoKeyStringValue) {
    JsonObject *jo = new JsonObject;
    Value * v1 = new StringValue("value1");
    jo->set("key1", v1);
    Value * v2 = new StringValue("value2");
    jo->set("key2", v2);
    
    ASSERT_EQ("\"value1\"", jo->getValue("key1")->toString());
    ASSERT_EQ("\"value2\"", jo->getValue("key2")->toString());
    ASSERT_EQ("{\n\"key1\":\"value1\",\n\"key2\":\"value2\"\n}", jo->toString());

    delete jo;
    delete v1;
    delete v2;
}

TEST(JSonSuite, Composite) {
    JsonObject *jo = new JsonObject;
    Value * v1 = new StringValue("value1");
    jo->set("key1", v1);
    Value * v2 = new StringValue("value2");
    jo->set("key2", v2);

    JsonObject *j_composite = new JsonObject;
    j_composite->set("keyc", jo);
    ASSERT_EQ(jo, j_composite->getValue("keyc"));
    ASSERT_EQ("{\n\"keyc\":{\n\"key1\":\"value1\",\n\"key2\":\"value2\"\n}\n}", j_composite->toString());

    delete jo;
    delete v1;
    delete v2;
}

TEST(JSonSuite, JsonObjectIterator)
{
    JsonObject *jo = new JsonObject;
    Value * v1 = new StringValue("value1");
    jo->set("key1", v1);
    Value * v2 = new StringValue("value2");
    jo->set("key2", v2);

    JsonIterator *it = jo->createIterator();
    it->first();
    ASSERT_EQ("key1",it->currentKey());
    ASSERT_EQ(v1,it->currentValue());
    it->next();
    ASSERT_EQ("key2",it->currentKey());
    ASSERT_EQ(v2,it->currentValue());
    it->next();
    ASSERT_TRUE(it->isDone());

    delete jo;
    delete v1;
    delete v2;
}

TEST(JSonSuite, beautifyVisitorStringValue)
{
    StringValue * v1 = new StringValue("value1");
    BeautifyVisitor *beautifyVisitor = new BeautifyVisitor();
    beautifyVisitor->visitStringValue(v1);
    ASSERT_EQ("\"value1\"",beautifyVisitor->getResult());

    delete v1;
    delete beautifyVisitor;
}

TEST(JSonSuite, beautifyVisitorJsonObject)
{
    JsonObject *jo1 = new JsonObject;
    Value * v1_1 = new StringValue("Design Patterns Elements of Reusable Object-Oriented Software");
    jo1->set("name", v1_1);
    Value * v1_2 = new StringValue("Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides");
    jo1->set("author", v1_2);

    JsonObject *jo2 = new JsonObject;
    Value * v2_1 = new StringValue("Clean Code");
    jo2->set("name",v2_1);
    Value * v2_2 = new StringValue("Robert C. Martin");
    jo2->set("author",v2_2);

    JsonObject *jo0 = new JsonObject;
    jo0->set("design pattern", jo1);
    jo0->set("clean code", jo2);

    JsonObject *jo = new JsonObject;
    jo->set("books", jo0);

    BeautifyVisitor *beautifyVisitor = new BeautifyVisitor();
    beautifyVisitor->visitJsonObject(jo);
    string ans="{\n    \"books\": {\n        \"clean code\": {\n            \"author\": \"Robert C. Martin\",\n            \"name\": \"Clean Code\"\n        },\n        \"design pattern\": {\n            \"author\": \"Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides\",\n            \"name\": \"Design Patterns Elements of Reusable Object-Oriented Software\"\n        }\n    }\n}";

    ASSERT_EQ(ans,beautifyVisitor->getResult());

    delete jo1;
    delete v1_1;
    delete v1_2;
    delete jo2;
    delete v2_1;
    delete v2_2;
    delete jo0;
    delete jo;
    delete beautifyVisitor;
}

TEST(JSonSuite, json_builder)
{
    JsonBuilder * builder = new JsonBuilder();
    builder->buildObject("");
    builder->buildObject("clean code");
    builder->buildValue("name","Clean code");
    builder->buildValue("author","Robert C. Martin");
    builder->endObject();
    builder->endObject();

    BeautifyVisitor *beautifyVisitor = new BeautifyVisitor();
    beautifyVisitor->visitJsonObject(builder->getJsonObject());

    string ans = "{\n    \"clean code\": {\n        \"author\": \"Robert C. Martin\",\n        \"name\": \"Clean code\"\n    }\n}";
    ASSERT_EQ(ans,beautifyVisitor->getResult());

    delete builder;
    delete beautifyVisitor;
}

TEST(JSonSuite, json_parser)
{
    string input="{    \"books\": {        \"clean code\": {            \"author\": \"Robert C. Martin\",            \"name\": \"Clean Code\"        },        \"design pattern\": {            \"author\": \"Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides\",            \"name\": \"Design Patterns Elements of Reusable Object-Oriented Software\"        }    }}";

    JsonScanner * scanner = new JsonScanner();
    JsonBuilder * builder = new JsonBuilder();
    JsonParser * parser = new JsonParser(scanner, builder);

    parser->setInput(input);
    parser->parse();
    
    BeautifyVisitor *beautifyVisitor = new BeautifyVisitor();
    beautifyVisitor->visitJsonObject(parser->getJsonObject());

    string ans="{\n    \"books\": {\n        \"clean code\": {\n            \"author\": \"Robert C. Martin\",\n            \"name\": \"Clean Code\"\n        },\n        \"design pattern\": {\n            \"author\": \"Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides\",\n            \"name\": \"Design Patterns Elements of Reusable Object-Oriented Software\"\n        }\n    }\n}";
    ASSERT_EQ(ans, beautifyVisitor->getResult());

    delete scanner;
    delete builder;
    delete parser;
    delete beautifyVisitor;
}


