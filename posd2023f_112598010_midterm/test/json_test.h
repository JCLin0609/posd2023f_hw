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
}

TEST(JSonSuite, TwoKeyStringValue) {
    JsonObject *jo = new JsonObject;
    Value * v1 = new StringValue("value1");
    jo->set("bkey1", v1);
    Value * v2 = new StringValue("value2");
    jo->set("akey2", v2);
    
    ASSERT_EQ("\"value1\"", jo->getValue("key1")->toString());
    ASSERT_EQ("\"value2\"", jo->getValue("key2")->toString());
    ASSERT_EQ("{\n\"key1\":\"value1\",\n\"key2\":\"value2\"\n}", jo->toString());
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
}