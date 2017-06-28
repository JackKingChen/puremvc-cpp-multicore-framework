
#include <unittest.h>

#include <PureMVC/PureMVC.hpp>

using PureMVC::Interfaces::IFacade;
using PureMVC::Interfaces::IProxy;
using PureMVC::Interfaces::IMediator;
using PureMVC::Interfaces::IAggregate;
using PureMVC::Interfaces::IIterator;
using PureMVC::Patterns::Facade;
using PureMVC::Patterns::Proxy;
using PureMVC::Patterns::Mediator;
using PureMVC::Interfaces::INotification;
using PureMVC::Patterns::SimpleCommand;

namespace testdata
{
    struct FacadeTestVO
    {
        int input;
        int result;

        FacadeTestVO(int input)
        {
            this->input = input;
            this->result = 0;
        }
    };

    struct SimpleTestCommand : public SimpleCommand
    {
        void execute(INotification const& notification)
        {
            FacadeTestVO& vo =  *(FacadeTestVO*) notification.getBody();
            vo.result = vo.input * 2;
        }
    };
}

UNITTEST(a)
{
    IFacade &facade = Facade::getInstance("FacadeTestKey1");
    testdata::SimpleTestCommand command;

    EXPECT_EQ("FacadeTestKey1",facade.getMultitonKey());

    facade.registerCommand("FacadeTestNote", &command);

    testdata::FacadeTestVO vo(32);
    facade.sendNotification("FacadeTestNote", &vo );

    EXPECT_EQ(64,vo.result);
}

UNITTEST(b)
{
    IFacade &facade = Facade::getInstance("FacadeTestKey1");
    testdata::SimpleTestCommand command;

    EXPECT_EQ("FacadeTestKey1",facade.getMultitonKey());

    facade.registerCommand("FacadeTestNote", &command);

    testdata::FacadeTestVO vo(32);
    facade.sendNotification("FacadeTestNote", &vo );

    EXPECT_EQ(64,vo.result);
}

