#include "catch.hpp"

#include <ocr/ocr.h>

namespace {

struct Fixture {

    Fixture() {
        _pool = ocr_pool_create(ocr_mb(32), NULL);
    }

    ~Fixture() {
        ocr_pool_destroy(_pool);
    }

    ocr_pool_t *_pool;
};

TEST_CASE_METHOD(Fixture, "simple boolean false", "[json]")
{
    ocr_json_t *json = ocr_json_parse(_pool, "false");
    REQUIRE(json);
    REQUIRE(json->type == OCR_JSON_BOOLEAN);
    REQUIRE(json->data.boolean.value == false);
}

TEST_CASE_METHOD(Fixture, "simple boolean true", "[json]")
{
    ocr_json_t *json = ocr_json_parse(_pool, "true");
    REQUIRE(json);
    REQUIRE(json->type == OCR_JSON_BOOLEAN);
    REQUIRE(json->data.boolean.value == true);
}

TEST_CASE_METHOD(Fixture, "simple null", "[json]")
{
    ocr_json_t *json = ocr_json_parse(_pool, "null");
    REQUIRE(json);
    REQUIRE(json->type == OCR_JSON_NULL);
}

TEST_CASE_METHOD(Fixture, "simple number", "[json]")
{
    ocr_json_t *json = ocr_json_parse(_pool, "1234");
    REQUIRE(json);
    REQUIRE(json->type == OCR_JSON_NUMBER);
    REQUIRE((u32)json->data.number.value == 1234);

}

TEST_CASE_METHOD(Fixture, "simple array", "[json]")
{
    ocr_json_t *json = ocr_json_parse(_pool, "[1,true,false,null]");
    REQUIRE(json);
    REQUIRE(json->type == OCR_JSON_ARRAY);
    REQUIRE(json->data.array.size == 4);
}

TEST_CASE_METHOD(Fixture, "simple object", "[json]")
{
    ocr_json_t *json = ocr_json_parse(_pool, "{\"enabled\": true}");
    REQUIRE(json);
    REQUIRE(json->type == OCR_JSON_OBJECT);
    REQUIRE(json->data.object.size == 1);

    ocr_json_t *enabled = ocr_json_get(json, "enabled");
    REQUIRE(enabled);
    REQUIRE(enabled->type == OCR_JSON_BOOLEAN);
    REQUIRE(enabled->data.boolean.value == true);
}

}
