#include "JSONGeometry.hpp"
#include <gtest/gtest.h>

#include <wkb/reader.hpp>

using namespace geopackage;

class WKB_Test : public ::testing::Test
{
  protected:
    WKB_Test() {}

    ~WKB_Test() override {}

    void SetUp() override {
        this->wkb["point"] = {
            0x01,
            0x01, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x40
        };

        this->wkb["point_big"] = {
            0x00,
            0x00, 0x00, 0x00, 0x01,
            0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x40, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        // LINESTRING(30 10, 10 30, 40 40)
        this->wkb["linestring"] = {
            0x01,
            0x02, 0x00, 0x00, 0x00,
            0x03, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x40,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x40,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x40,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x40,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x40,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x40
        };
        
        this->wkb["polygon"] = {
            0x01,
            0x03, 0x00, 0x00, 0x00,
            0x01, 0x00, 0x00, 0x00,
            0x05, 0x00, 0x00, 0x00,
            0x54, 0xE3, 0xA5, 0x9B, 0xC4, 0x60, 0x25, 0x40,
            0x64, 0x3B, 0xDF, 0x4F, 0x8D, 0x17, 0x39, 0xC0,
            0x5C, 0x8F, 0xC2, 0xF5, 0x28, 0x4C, 0x41, 0x40,
            0xEC, 0x51, 0xB8, 0x1E, 0x85, 0x2B, 0x34, 0xC0,
            0xD5, 0x78, 0xE9, 0x26, 0x31, 0x68, 0x43, 0x40,
            0x6F, 0x12, 0x83, 0xC0, 0xCA, 0xD1, 0x41, 0xC0,
            0x1B, 0x2F, 0xDD, 0x24, 0x06, 0x01, 0x2B, 0x40,
            0xA4, 0x70, 0x3D, 0x0A, 0xD7, 0x93, 0x43, 0xC0,
            0x54, 0xE3, 0xA5, 0x9B, 0xC4, 0x60, 0x25, 0x40,
            0x64, 0x3B, 0xDF, 0x4F, 0x8D, 0x17, 0x39, 0xC0
        };

        // MULTIPOINT(10 40,40 30,20 20,30 10)
        this->wkb["multipoint"] = {
            0x01,
            0x04, 0x00, 0x00, 0x00,
            0x04, 0x00, 0x00, 0x00,
            0x01,
            0x01, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x40,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x40,
            0x01,
            0x01, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x40,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x40,
            0x01,
            0x01, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x40,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x40,
            0x01,
            0x01, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x40,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x40
        };

        // MULTILINESTRING((10 10,20 20,10 40),(40 40,30 30,40 20,30 10))
        this->wkb["multilinestring"] = {
            0x01,
            0x05, 0x00, 0x00, 0x00,
            0x02, 0x00, 0x00, 0x00,
            0x01,
            0x02, 0x00, 0x00, 0x00,
            0x03, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x40,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x40,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x40,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x40,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x40,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x40,
            0x01,
            0x02, 0x00, 0x00, 0x00,
            0x04, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x40,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x40,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x40,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x40,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x40,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x40,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x40,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x40
        };
    }

    void TearDown() override {}
    
    std::map<std::string, std::vector<wkb::byte_t>> wkb;
};

TEST_F(WKB_Test, wkb_point_test) // also tests endianness
{
    const auto geom_big    = wkb::read_known_wkb<geojson::coordinate_t>(this->wkb["point_big"]);
    const auto geom_little = wkb::read_known_wkb<geojson::coordinate_t>(this->wkb["point"]);
    EXPECT_NEAR(geom_big.get<0>(), geom_little.get<0>(), 0.000001);
    EXPECT_NEAR(geom_big.get<1>(), geom_little.get<1>(), 0.000001);
    EXPECT_NEAR(geom_big.get<0>(), 2.0, 0.000001);
    EXPECT_NEAR(geom_big.get<1>(), 4.0, 0.000001);
    EXPECT_NEAR(geom_big.get<0>(), 2.0, 0.000001);
    EXPECT_NEAR(geom_big.get<1>(), 4.0, 0.000001);
}

TEST_F(WKB_Test, wkb_linestring_test)
{
    const geojson::geometry geom = wkb::read_wkb(this->wkb["linestring"]);
    EXPECT_EQ(geom.which() + 1, 2);

    const geojson::linestring_t& line = boost::get<geojson::linestring_t>(geom);
    const std::vector<std::pair<double, double>> expected_coordinates = {
        {30, 10}, {10, 30}, {40, 40}
    };

    for (int i = 0; i < expected_coordinates.size(); i++) {
        EXPECT_NEAR(line[i].get<0>(), expected_coordinates[i].first, 0.0001);
        EXPECT_NEAR(line[i].get<1>(), expected_coordinates[i].second, 0.0001);
    }
}

TEST_F(WKB_Test, wkb_polygon_test)
{    
    const geojson::geometry geom = wkb::read_wkb(this->wkb["polygon"]);
    EXPECT_EQ(geom.which() + 1, 3); // +1 since variant.which() is 0-based

    const geojson::polygon_t& poly = boost::get<geojson::polygon_t>(geom);
    const std::vector<std::pair<double, double>> expected_coordinates = {
        {10.689, -25.092},
        {34.595, -20.170},
        {38.814, -35.639},
        {13.502, -39.155},
        {10.689, -25.092}
    };

    for (int i = 0; i < expected_coordinates.size(); i++) {
        EXPECT_NEAR(poly.outer().at(i).get<0>(), expected_coordinates[i].first, 0.0001);
        EXPECT_NEAR(poly.outer().at(i).get<1>(), expected_coordinates[i].second, 0.0001);
    }
}

TEST_F(WKB_Test, wkb_multipoint_test)
{
    const geojson::geometry geom = wkb::read_wkb(this->wkb["multipoint"]);
    EXPECT_EQ(geom.which() + 1, 4);

    const geojson::multipoint_t& mp = boost::get<geojson::multipoint_t>(geom);
    const std::vector<std::pair<double, double>> expected_coordinates = {
        {10, 40}, {40, 30}, {20, 20}, {30, 10}
    };

    for (int i = 0; i < expected_coordinates.size(); i++) {
        EXPECT_NEAR(mp[i].get<0>(), expected_coordinates[i].first, 0.0001);
        EXPECT_NEAR(mp[i].get<1>(), expected_coordinates[i].second, 0.0001);
    }
}

TEST_F(WKB_Test, wkb_multilinestring_test)
{
    const geojson::geometry geom = wkb::read_wkb(this->wkb["multilinestring"]);
    EXPECT_EQ(geom.which() + 1, 5);

    const geojson::multilinestring_t& mp = boost::get<geojson::multilinestring_t>(geom);
    const std::vector<std::vector<std::pair<double, double>>> expected_coordinates = {
        { {10, 10}, {20, 20}, {10, 40} },
        { {40, 40}, {30, 30}, {40, 20}, {30, 10} }
    };

    for (int i = 0; i < expected_coordinates.size(); i++) {
        for (int j = 0; j < expected_coordinates[i].size(); j++) {
            EXPECT_NEAR(mp[i][j].get<0>(), expected_coordinates[i][j].first, 0.0001);
            EXPECT_NEAR(mp[i][j].get<1>(), expected_coordinates[i][j].second, 0.0001);
        }
    }
}