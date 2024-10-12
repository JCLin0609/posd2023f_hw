#pragma once


class JsonBuilder {
public:
    void buildValue(std::string key, std::string value);

    void buildObject(std::string key);

    void endObject();

    JsonObject * getJsonObject();
};
