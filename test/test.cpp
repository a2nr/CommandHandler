#include <iostream>
#include "gtest/gtest.h"

#include "CommandClass.h"

#define API_TEST_MAX_INDEX_BUFFER 0xff + 5

class apiTest;

class testCmd1 : public user_command<apiTest>

{
  public:
    testCmd1() {}
    unsigned short exe(CommandHandler<apiTest> *cmd)
    {
        unsigned char c[5] = {0x0A, 0x01, 0x0B, 0x0C, 0x00};
        unsigned char f;
        if (memcmp(cmd->getHeader(), c, 5) != 0)
            f = 0;
        else
        {
            cmd->push(c, 5);
            f = 1;
        }
        return cmd->Return(f, 0);

    }
    const char *getCommand(void)
    {
        return "\x01";
    }
};

class testCmd2 : public user_command<apiTest>

{
  public:
    testCmd2() {}
    unsigned short exe(CommandHandler<apiTest> *cmd)
    {
        return 0;
    }
    const char *getCommand(void)
    {
        return "\x02";
    }
};

class apiTest : public ::testing::Test
{

  public:
    unsigned char command[API_TEST_MAX_INDEX_BUFFER];
    unsigned char *ptrParam;
    unsigned long *ptrStatusResponds;
    unsigned int len;
    unsigned char r;
    unsigned int m;
    CommandHandler<apiTest> cmd;
    user_command<apiTest> *ucTable[4];

    unsigned char inc(CommandHandler<apiTest> *cmd)
    {
        cmd->push(command, m + 1 - len);
        return r;
    }
    unsigned char out(CommandHandler<apiTest> *cmd)
    {
        ptrParam = (unsigned char *)cmd->getParam();
        ptrStatusResponds = cmd->getStatusResponds();
        return r;
    }
    void err(unsigned char c)
    {
    }
    apiTest()
    {
        struct cm_init_class<apiTest> init;

        init.inc = &apiTest::inc;
        init.out = &apiTest::out;
        init.err = &apiTest::err;
        init.all_user_command = ucTable;
        init.length_user_command = sizeof(ucTable) / sizeof(user_command<apiTest> *);

        cmd.init(this, &init);
    }
    void SetUp() override
    {
    }

    // void TearDown() override
    // {
    // }
};

TEST_F(apiTest, API_Incoming_invoke)
{
    unsigned char *ptrBUffe = cmd[0];
    memcpy(command, "test1", 5);
    len = 5;
    m = (len * 2) - 1;

    r = 0;
    cmd.run();
    EXPECT_EQ(memcmp(ptrBUffe, "test1", 5), 0);
}
TEST_F(apiTest, API_User_Command_and_responds_invoke)
{
    unsigned char *ptrBUffe = cmd[0];
    unsigned char c[5] = {0x0A, 0x01, 0x0B, 0x0C, 0x00};
    unsigned long sw = 0x0503;
    testCmd1 _123;
    memcpy(command, c, 5);
    ucTable[0] = &_123;
    ucTable[1] = &_123;
    ucTable[2] = &_123;
    ucTable[3] = &_123;
    len = 5;
    m = (len * 2) - 1;

    r = 1;
    cmd.run();
    EXPECT_EQ(memcmp(ptrParam, &c[0], 5), 0);
    EXPECT_EQ(*ptrStatusResponds, sw);
}
TEST_F(apiTest, API_availbe_count_1_0x104)
{

    m = API_TEST_MAX_INDEX_BUFFER;
    r = 0;

    memset(command, 0xff, m);

    len = m + 1;
    while (len--)
    {
        cmd.run();
        EXPECT_EQ(cmd.available(), m + 1 - len);
    }
}
