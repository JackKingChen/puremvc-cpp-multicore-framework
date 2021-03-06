/*******************************************************************
*
*    DESCRIPTION:Copyright(c) 2014-2024 KaoPu Network Technology Co,.Ltd
*
*    AUTHOR: chenxianfeng (ebdsoft@163.com)
*
*    HISTORY:
*
*    DATE:2015-05-10
*
*******************************************************************/

#ifndef __UNITTEST_H__
#define __UNITTEST_H__

#include "host.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
namespace unittest
{
    /*
    * --------------------------------------------------------
    * The helper function for {ASSERT|EXPECT}_EQ.
    * --------------------------------------------------------
    */
    template <typename T1, typename T2>
    bool UnitTest_Helper_Failure(const char* expected_expression,
        const char* actual_expression,
        const char* operate_expression,
        const T1& expected,const T2& actual) 
    {
        OSTTY::color(OSTTY::RED);
        std::cout<<"[FAILED  ]"<<expected<<" "<<operate_expression<<" "<<actual<<"="<<actual_expression<<"\n";
        OSTTY::color(OSTTY::WHITE);
        return false;
    }

    template <typename T1, typename T2>
    bool UnitTest_Helper_Success(const char* expected_expression,
        const char* actual_expression,
        const char* operate_expression,
        const T1& expected,const T2& actual) 
    {
        OSTTY::color(OSTTY::GREEN);
        std::cout<<"[PASSED  ]"<<expected<<" "<<operate_expression<<" "<<actual<<"="<<actual_expression<<"\n";
        OSTTY::color(OSTTY::WHITE);
        return true;
    }

    template <typename T1, typename T2>
    bool UnitTest_Helper_Abort(const char* expected_expression,
        const char* actual_expression,
        const char* operate_expression,
        const T1& expected,const T2& actual)
    {
        OSTTY::color(OSTTY::RED);
        std::cout<<"[ABORT  ]"<<"\n";
        OSTTY::color(OSTTY::WHITE);
        return true;
    }

    /************************************************************************/
    /*                                                                      */
    /************************************************************************/
    /* INTERNAL IMPLEMENTATION - DO NOT USE IN A USER PROGRAM.*/

    /*
    * Temporarily disables warning on
    * signed/unsigned mismatch.
    */
#define UNITTEST_IMPL_CMP_HELPER_(op_name, op)\
    template <typename T1, typename T2> \
    bool UnitTest_Helper_Compare ##op_name(const char* expected_expression,const char* actual_expression, \
        const T1& expected,const T2& actual,bool show_success) { \
            if ((T2)expected op actual) { \
                return show_success?UnitTest_Helper_Success(expected_expression,actual_expression,#op,expected,actual):true; \
            }else{ \
                return UnitTest_Helper_Failure(expected_expression,actual_expression,#op,expected,actual); \
        } \
    }

    // Implements the helper function for {ASSERT|EXPECT}_EQ
    UNITTEST_IMPL_CMP_HELPER_(EQ, ==)
    // Implements the helper function for {ASSERT|EXPECT}_NE
    UNITTEST_IMPL_CMP_HELPER_(NE, !=)
    // Implements the helper function for {ASSERT|EXPECT}_LE
    UNITTEST_IMPL_CMP_HELPER_(LE, <=)
    // Implements the helper function for {ASSERT|EXPECT}_LT
    UNITTEST_IMPL_CMP_HELPER_(LT, < )
    // Implements the helper function for {ASSERT|EXPECT}_GE
    UNITTEST_IMPL_CMP_HELPER_(GE, >=)
    // Implements the helper function for {ASSERT|EXPECT}_GT
    UNITTEST_IMPL_CMP_HELPER_(GT, > )

    /*
    * --------------------------------------------------------
    * virtual class of unit test
    * --------------------------------------------------------
    */
    class UnitTest:public OSCmd
    {
    protected:
        virtual const char* Name(void){return "";};
        virtual const char* Help(void){return "";};
        virtual bool        Info(void){return true; };
        virtual bool        Time(void){return false;};
        virtual bool        Hide(void){return false;};

        virtual void Test (int argc, const char* argv[])=0;
        virtual void Setup(){};
        virtual void Clean(){};

    protected:

        virtual int cmd_action(int argc, const char* argv[])
        {
            /*show info*/
            if(Info())
                OSTTY::print(OSTTY::YELLOW," run %s ...\n",Name());
            /*clear for each case*/
            test_abort=false;
            /*run test*/
            Setup();
            Test(argc,argv);
            Clean();
            return 0;
        };
        virtual const char*cmd_help(void)
        {return Help();}
        virtual const char*cmd_name(void)
        {return Name();};
        virtual bool       cmd_hide(void)
        {return Hide();};

    public:
        static bool test_abort;
    };

}; /*end of namespace unittest*/

/************************************************************************/
/*                                                                      */
/************************************************************************/

/*
* simple unit test helper
*/
#define UNITTEST_NAME(test_case,test_name) \
    test_case ##_ ##test_name ##_Test

#define UNITTEST_INST(test_case,test_name) \
    the ##test_case ##_ ##test_name ##_Test

#define UNITTEST_CLASS(test_base,test_name,test_hide,...) \
    class UNITTEST_NAME(test_base,test_name):public test_base \
    { \
    public: \
        UNITTEST_NAME(test_base,test_name) \
        (const char* name,bool info=true,const char* help=NULL) \
        {\
            this_name = name;\
            this_help = help?help:"no help";\
            this_info = info;\
            this_hide = test_hide;\
        };\
    protected: \
        virtual void        Test(int argc, const char* argv[]);\
        virtual const char* Help(){return this_help;};\
        virtual const char* Name(){return this_name;};\
        virtual bool        Info(){return this_info;};\
        virtual bool        Hide(){return this_hide;};\
    protected: \
        const char *this_name;\
        const char *this_help;\
        bool        this_info;\
        bool        this_hide;\
    }; \
    static UNITTEST_NAME(test_base,test_name)  \
        UNITTEST_INST(test_base,test_name)(#test_name,##__VA_ARGS__);\
    void UNITTEST_NAME(test_base,test_name)::Test(int argc, const char* argv[])


#define UNITTEST(test_name,...) \
    UNITTEST_CLASS(UnitTest,test_name,false,##__VA_ARGS__)

#define UNITTEST_HIDE(test_name,...) \
    UNITTEST_CLASS(UnitTest,test_name,true,##__VA_ARGS__)

#define UNITTEST_INHERIT(test_base,test_name,...) \
    UNITTEST_CLASS(test_base,test_name,false,##__VA_ARGS__)

#define UNITTEST_INHERIT_HIDE(test_base,test_name,...) \
    UNITTEST_CLASS(test_base,test_name,true,##__VA_ARGS__)

/*
* helper micro
*/

#define UNITTEST_CHECK(_ret,_exp,_ops,_abort,_show) \
    if(!UnitTest::test_abort)do{ \
        if(!_ops(#_ret,#_exp,_ret,_exp,_show)){ \
            if(_abort){ \
                UnitTest::test_abort=true;\
                ::unittest::UnitTest_Helper_Abort(NULL,NULL,NULL,0,0); \
            } \
        } \
    }while(0)


#define EXPECT_CHECK(_ret,_exp,_cmp) \
    UNITTEST_CHECK(_ret,_exp,_cmp,false,true)

#define ASSERT_CHECK(_ret,_exp,_cmp) \
    UNITTEST_CHECK(_ret,_exp,_cmp,true, true)


#define EXPECT_CHECK_QUIET(_ret,_exp,_cmp) \
    UNITTEST_CHECK(_ret,_exp,_cmp,false,false)

#define ASSERT_CHECK_QUIET(_ret,_exp,_cmp) \
    UNITTEST_CHECK(_ret,_exp,_cmp,true, false)

/*
* real micro
*/
#define EXPECT_EQ(_ret,_exp)   EXPECT_CHECK(_ret,_exp,::unittest::UnitTest_Helper_CompareEQ)
#define EXPECT_NE(_ret,_exp)   EXPECT_CHECK(_ret,_exp,::unittest::UnitTest_Helper_CompareNE)
#define EXPECT_LE(_ret,_exp)   EXPECT_CHECK(_ret,_exp,::unittest::UnitTest_Helper_CompareLE)
#define EXPECT_LT(_ret,_exp)   EXPECT_CHECK(_ret,_exp,::unittest::UnitTest_Helper_CompareLT)
#define EXPECT_GE(_ret,_exp)   EXPECT_CHECK(_ret,_exp,::unittest::UnitTest_Helper_CompareGE)
#define EXPECT_GT(_ret,_exp)   EXPECT_CHECK(_ret,_exp,::unittest::UnitTest_Helper_CompareGT)
#define EXPECT_TRUE(_exp)      EXPECT_EQ(true,!!(_exp))

#define ASSERT_EQ(_ret,_exp)   ASSERT_CHECK(_ret,_exp,::unittest::UnitTest_Helper_CompareEQ)
#define ASSERT_NE(_ret,_exp)   ASSERT_CHECK(_ret,_exp,::unittest::UnitTest_Helper_CompareNE)
#define ASSERT_LE(_ret,_exp)   ASSERT_CHECK(_ret,_exp,::unittest::UnitTest_Helper_CompareLE)
#define ASSERT_LT(_ret,_exp)   ASSERT_CHECK(_ret,_exp,::unittest::UnitTest_Helper_CompareLT)
#define ASSERT_GE(_ret,_exp)   ASSERT_CHECK(_ret,_exp,::unittest::UnitTest_Helper_CompareGE)
#define ASSERT_GT(_ret,_exp)   ASSERT_CHECK(_ret,_exp,::unittest::UnitTest_Helper_CompareGT)
#define ASSERT_TRUE(_exp)      ASSERT_EQ(true,!!(_exp))


#define QUIET_EXPECT_EQ(_ret,_exp)   EXPECT_CHECK_QUIET(_ret,_exp,::unittest::UnitTest_Helper_CompareEQ)
#define QUIET_EXPECT_NE(_ret,_exp)   EXPECT_CHECK_QUIET(_ret,_exp,::unittest::UnitTest_Helper_CompareNE)
#define QUIET_EXPECT_LE(_ret,_exp)   EXPECT_CHECK_QUIET(_ret,_exp,::unittest::UnitTest_Helper_CompareLE)
#define QUIET_EXPECT_LT(_ret,_exp)   EXPECT_CHECK_QUIET(_ret,_exp,::unittest::UnitTest_Helper_CompareLT)
#define QUIET_EXPECT_GE(_ret,_exp)   EXPECT_CHECK_QUIET(_ret,_exp,::unittest::UnitTest_Helper_CompareGE)
#define QUIET_EXPECT_GT(_ret,_exp)   EXPECT_CHECK_QUIET(_ret,_exp,::unittest::UnitTest_Helper_CompareGT)
#define QUIET_EXPECT_TRUE(_exp)      QUIET_EXPECT_EQ(true,!!(_exp))

#define QUIET_ASSERT_EQ(_ret,_exp)   ASSERT_CHECK_QUIET(_ret,_exp,::unittest::UnitTest_Helper_CompareEQ)
#define QUIET_ASSERT_NE(_ret,_exp)   ASSERT_CHECK_QUIET(_ret,_exp,::unittest::UnitTest_Helper_CompareNE)
#define QUIET_ASSERT_LE(_ret,_exp)   ASSERT_CHECK_QUIET(_ret,_exp,::unittest::UnitTest_Helper_CompareLE)
#define QUIET_ASSERT_LT(_ret,_exp)   ASSERT_CHECK_QUIET(_ret,_exp,::unittest::UnitTest_Helper_CompareLT)
#define QUIET_ASSERT_GE(_ret,_exp)   ASSERT_CHECK_QUIET(_ret,_exp,::unittest::UnitTest_Helper_CompareGE)
#define QUIET_ASSERT_GT(_ret,_exp)   ASSERT_CHECK_QUIET(_ret,_exp,::unittest::UnitTest_Helper_CompareGT)
#define QUIET_ASSERT_TRUE(_exp)      QUIET_ASSERT_EQ(true,!!(_exp))

/************************************************************************/
/* for PCM data debug                                                   */
/************************************************************************/
#define  PCMCAPTURE(_name,_lpcm,_rpcm,_size,_file,...) \
    do { \
        static WaveFile dbg_ ##_name; \
        if(!dbg_ ##_name.is_open()) \
            dbg_ ##_name.Create(_file,##__VA_ARGS__); \
        dbg_ ##_name.Write(_lpcm,_rpcm,_size); \
        dbg_ ##_name.Update(); \
    } while (0)

#define  PCMPLAYOUT(_name,_lpcm,_rpcm,_size,_file) \
    do { \
        static WaveFile dbg_#_name;\
        if(!dbg_#_name.is_open())  \
            dbg_#_name.Open(file); \
        if(dbg_#_name.Read(_lpcm,_rpcm,_size)<_size){ \
            dbg_#_name.Seek(0); \
            dbg_#_name.Read(_lpcm,_rpcm,_size); \
        }   \
    } while (0)

/************************************************************************/
/*                                                                      */
/************************************************************************/
using namespace unittest;

/************************************************************************/
/*                                                                      */
/************************************************************************/
#endif /*__UNITTEST_H__*/
