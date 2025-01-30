#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "crc.hpp" 

class CRCTest : public ::testing::Test {
protected:
  
    CRC16 crc;

    
    void SetUp() override {
        
    }

    
    void TearDown() override {
       
    }
};


TEST_F(CRCTest, ModbusCRC16Valid) {
    uint8_t data[] = {0x01, 0x03, 0x00, 0x02, 0x00, 0x03, 0xA4, 0x0B};

    uint16_t expected_crc = 0x0BA4;

    uint16_t computed_crc = crc.CalculateCRC(data, sizeof(data) - 2);

    EXPECT_EQ(computed_crc, expected_crc);
}

TEST_F(CRCTest, ModbusCRC16EmptyData) {
    uint8_t empty_data[] = {};
    uint16_t expected_crc = 0xFFFF; 

    uint16_t computed_crc = crc.CalculateCRC(empty_data, sizeof(empty_data));


    EXPECT_EQ(computed_crc, expected_crc);
}

TEST_F(CRCTest, ModbusCRC16Response) {
    uint8_t response_data[] = { 0x01, 0x03, 0x06,  0x00, 0x01, 0x00, 0x02, 0x00, 0x64, 0xBC, 0x9E };


    uint16_t expected_crc = 0x9EBC; 

    
    uint16_t computed_crc = crc.CalculateCRC(response_data, sizeof(response_data) - 2); 


    EXPECT_EQ(computed_crc, expected_crc);
}

TEST_F(CRCTest, ModbusCRC16_Modify) {
    uint8_t modify_data[] = { 0x01, 0x06, 0x00,  0x00, 0x00, 0x02, 0x08, 0x0B};


    uint16_t expected_crc = 0x0B08; 

    
    uint16_t computed_crc = crc.CalculateCRC(modify_data, sizeof(modify_data) - 2); 


    EXPECT_EQ(computed_crc, expected_crc);
}

// invalid
TEST_F(CRCTest, ModbusCRC16_Invalid){
    uint8_t invalid_data[] = {0x01, 0x06, 0x00, 0x07, 0x21, 0x34, 0x21,  0x8C};

    uint16_t expected_crc = 0x218C; 

    uint16_t computed_crc = crc.CalculateCRC(invalid_data, sizeof(invalid_data) - 2);

    EXPECT_NE(computed_crc, expected_crc);

}

void setup()
{
    Serial.begin(115200);
    delay(1000);
    ::testing::InitGoogleTest();
       if (RUN_ALL_TESTS());
}
void loop()
{
    
}
