#include <iostream>
#include "gtest/gtest.h"
#define COMMAND_CLASS_MAX_COMMAND 4
#include "CommandClass.h"

class initTest;

unsigned short (*ptrFncSwitchTest)(CommandHandler<initTest> *cmd) = NULL;

static const char *ptrGetCommand;

class initCmd : public user_command<initTest>
{
  public:
    initCmd() {}
    unsigned short exe(CommandHandler<initTest> *cmd)
    {
        unsigned short re = 0;
        if (ptrFncSwitchTest != NULL)
            re = ptrFncSwitchTest(cmd);
        return re;
    }
    const char *getCommand(void)
    {
        return ptrGetCommand;
    }
};

class initTest : public ::testing::Test
{
  public:
    cm_init_class<initTest> cInit;
    initCmd userCmd1;
    initCmd userCmd3;
    initCmd userCmd2;
    initCmd userCmd4;
    user_command<initTest> *all_cmd[4];
    CommandHandler<initTest> cmd[4];

    unsigned char inc(CommandHandler<initTest> *cmd)
    {
        return 0;
    }
    unsigned char out(CommandHandler<initTest> *cmd)
    {
        return 0;
    }
    void err(unsigned char c)
    {
    }
    void SetUp() override
    {
        all_cmd[0] = &userCmd1;
        all_cmd[1] = &userCmd2;
        all_cmd[2] = &userCmd3;
        all_cmd[3] = &userCmd4;
        cInit.inc = &initTest::inc;
        cInit.out = &initTest::out;
        cInit.err = &initTest::err;
        cInit.all_user_command = all_cmd;
        cInit.length_user_command = sizeof(all_cmd) / sizeof(user_command<initTest> *);
        cmd[0].init(this, &cInit);
        cmd[1].init(this, &cInit);
        cmd[2].init(this, &cInit);
        cmd[3].init(this, &cInit);
    }
};

TEST_F(initTest, check_allocated_buffer_multi_command)
{
    unsigned char *ptrSBuffer_head;
    unsigned char *ptrSBuffer_tail;
    unsigned char *ptrSBuffer_nextRef;

    ptrSBuffer_head = cmd[0][0];
    ptrSBuffer_tail = cmd[0][COMMAND_CLASS_SIZE_BUFFER - 1];
    EXPECT_EQ(ptrSBuffer_tail - ptrSBuffer_head, COMMAND_CLASS_SIZE_BUFFER - 1);
    ptrSBuffer_nextRef = cmd[1][0];
    EXPECT_GT(ptrSBuffer_nextRef, ptrSBuffer_tail);

    ptrSBuffer_head = cmd[1][0];
    ptrSBuffer_tail = cmd[1][COMMAND_CLASS_SIZE_BUFFER - 1];
    EXPECT_EQ(ptrSBuffer_tail - ptrSBuffer_head, COMMAND_CLASS_SIZE_BUFFER - 1);
    ptrSBuffer_nextRef = cmd[2][0];
    EXPECT_GT(ptrSBuffer_nextRef, ptrSBuffer_tail);

    ptrSBuffer_head = cmd[2][0];
    ptrSBuffer_tail = cmd[2][COMMAND_CLASS_SIZE_BUFFER - 1];
    EXPECT_EQ(ptrSBuffer_tail - ptrSBuffer_head, COMMAND_CLASS_SIZE_BUFFER - 1);
    ptrSBuffer_nextRef = cmd[3][0];
    EXPECT_GT(ptrSBuffer_nextRef, ptrSBuffer_tail);

    ptrSBuffer_head = cmd[3][0];
    ptrSBuffer_tail = cmd[3][COMMAND_CLASS_SIZE_BUFFER - 1];
    EXPECT_EQ(ptrSBuffer_tail - ptrSBuffer_head, COMMAND_CLASS_SIZE_BUFFER - 1);

    CommandHandler<initTest> cmd_try;
    ptrSBuffer_nextRef = cmd_try[0];
    EXPECT_EQ(ptrSBuffer_nextRef, nullptr);
}


int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}