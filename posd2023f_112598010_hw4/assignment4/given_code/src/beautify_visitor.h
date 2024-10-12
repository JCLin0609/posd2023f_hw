#pragma once

class BeautifyVisitor : public JsonVisitor {
public:
    void visitJsonObject(JsonObject * obj);

    void visitStringValue(StringValue * val);

    std::string getResult() const;
};