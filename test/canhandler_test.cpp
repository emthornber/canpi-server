#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "canHandler.h"

class MockNodeConfigurator : public nodeConfigurator
{
public:
    MOCK_METHOD(int, setCanID, (int), (override));
    MOCK_METHOD(int, getCanID, (), (const, override));
    MOCK_METHOD(int, getNodeMode, (), (const, override));
};

class CanHandlerTest : public ::testing::Test
{
protected:
    std::unique_ptr<canHandler> handler;
    std::unique_ptr<MockNodeConfigurator> mockConfig;
    std::unique_ptr<log4cpp::Category> logger;

    void SetUp() override
    {
        logger = std::make_unique<log4cpp::Category>("TestLogger");
        mockConfig = std::make_unique<MockNodeConfigurator>();
        handler = std::make_unique<canHandler>(logger.get(), 1);
        handler->setConfigurator(mockConfig.get());
    }
};

TEST_F(CanHandlerTest, TestDoSelfEnum)
{
    // Arrange
    EXPECT_CALL(*mockConfig, getCanID()).WillOnce(testing::Return(5));

    // Act
    handler->doSelfEnum();

    // Assert
    EXPECT_TRUE(handler->isAutoEnumMode());

    // Verify that a CAN frame with RTR flag was added to the out_msgs queue
    ASSERT_FALSE(handler->getOutMsgs().empty());
    auto frame = handler->getOutMsgs().front().getFrame();
    EXPECT_EQ(frame.can_id, CAN_RTR_FLAG | 5);
    EXPECT_EQ(frame.can_dlc, 0);
}

TEST_F(CanHandlerTest, TestFinishSelfEnumWithNoConflict)
{
    // Arrange
    handler->doSelfEnum();
    EXPECT_CALL(*mockConfig, setCanID(testing::_)).WillOnce(testing::Return(true));
    EXPECT_CALL(*mockConfig, getNodeMode()).WillOnce(testing::Return(MTYP_SLIM));

    // Act
    handler->finishSelfEnum(0);

    // Assert
    EXPECT_FALSE(handler->isAutoEnumMode());
    EXPECT_EQ(handler->getCanId(), 1); // Assuming 1 is the default CanID

    // Verify that no NNACK was sent (as we're in SLIM mode)
    EXPECT_TRUE(handler->getOutMsgs().empty());
}

TEST_F(CanHandlerTest, TestFinishSelfEnumWithConflict)
{
    // Arrange
    handler->doSelfEnum();
    handler->setNodeNumber(1234); // Set a node number for NNACK
    EXPECT_CALL(*mockConfig, setCanID(testing::_)).WillOnce(testing::Return(true));
    EXPECT_CALL(*mockConfig, getNodeMode()).WillOnce(testing::Return(MTYP_FLIM));

    // Act
    handler->finishSelfEnum(1); // Simulate conflict with CanID 1
    handler->finishSelfEnum(2); // Simulate conflict with CanID 2
    handler->finishSelfEnum(0); // Finish enumeration

    // Assert
    EXPECT_FALSE(handler->isAutoEnumMode());
    EXPECT_EQ(handler->getCanId(), 3); // Should choose the next available CanID

    // Verify that NNACK was sent
    ASSERT_FALSE(handler->getOutMsgs().empty());
    auto frame = handler->getOutMsgs().front().getFrame();
    EXPECT_EQ(frame.data[0], OPC_NNACK);
    EXPECT_EQ(frame.data[1], 0x04); // High byte of node number (1234)
    EXPECT_EQ(frame.data[2], 0xD2); // Low byte of node number (1234)
}

TEST_F(CanHandlerTest, TestFinishSelfEnumWithAllConflicts)
{
    // Arrange
    handler->doSelfEnum();
    handler->setNodeNumber(1234);                            // Set a node number for CMDERR
    EXPECT_CALL(*mockConfig, setCanID(testing::_)).Times(0); // Should not be called
    EXPECT_CALL(*mockConfig, getNodeMode()).WillOnce(testing::Return(MTYP_FLIM));

    // Act
    for (int i = 1; i <= 99; ++i)
    {
        handler->finishSelfEnum(i); // Simulate conflicts with all possible CanIDs
    }
    handler->finishSelfEnum(0); // Finish enumeration

    // Assert
    EXPECT_FALSE(handler->isAutoEnumMode());
    EXPECT_EQ(handler->getCanId(), 0); // Should not have allocated a CanID

    // Verify that CMDERR was sent
    ASSERT_FALSE(handler->getOutMsgs().empty());
    auto frame = handler->getOutMsgs().front().getFrame();
    EXPECT_EQ(frame.data[0], OPC_CMDERR);
    EXPECT_EQ(frame.data[1], 0x04); // High byte of node number (1234)
    EXPECT_EQ(frame.data[2], 0xD2); // Low byte of node number (1234)
    EXPECT_EQ(frame.data[3], CMDERR_INVALID_EVENT);
}
