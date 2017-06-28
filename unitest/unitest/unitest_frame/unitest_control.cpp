
#include <unittest.h>

#include <PureMVC/PureMVC.hpp>

using PureMVC::Thread;
using PureMVC::Interfaces::IController;
using PureMVC::Interfaces::INotification;
using PureMVC::Interfaces::IView;
using PureMVC::Interfaces::IIterator;
using PureMVC::Interfaces::IAggregate;
using PureMVC::Core::Controller;
using PureMVC::Core::View;
using PureMVC::Patterns::Notification;
using PureMVC::Patterns::Notification;
using PureMVC::Patterns::DelegateCommand;
using PureMVC::Patterns::createCommand;

namespace data
{
    using PureMVC::Patterns::SimpleCommand;
    using PureMVC::Interfaces::INotification;
    using PureMVC::Interfaces::ICommand;

    struct ControllerTestVO
    {
        int input;
        int result;
        /**
         * Constructor.
         * 
         * @param input the number to be fed to the ControllerTestCommand
         */
        ControllerTestVO(int input)
        {
            this->input = input;
            this->result = 0;
        }
    };
    /**
     * A SimpleCommand subclass used by ControllerTest.
     */
    struct ControllerTestCommand
        : public virtual ICommand
        , public SimpleCommand
    {
        /**
         * Constructor.
         */
        ControllerTestCommand(void)
            :SimpleCommand()
        { }
        
        /**
         * Fabricate a result by multiplying the input by 2
         * 
         * @param note the note carrying the ControllerTestVO
         */
        virtual void execute(INotification const& note)
        {
            ControllerTestVO& vo= *(ControllerTestVO*)note.getBody();
            // Fabricate a result
            vo.result = 2 * vo.input;
        }
    };
}

UNITTEST(control)
{
    IController& controller = Controller::getInstance("ControllerTestKey1");

    static data::ControllerTestCommand command;

    for (int i=0;i<10; i++)
    {
        char szCtrKey[64] = { 0 };
        sprintf(szCtrKey,"ControllerTest%d",i);
        controller.registerCommand(szCtrKey, &command);
    }

    for (int i=0; i<10; i++)
    {
        char szCtrKey[64] = { 0 };
        sprintf(szCtrKey,"ControllerTest%d",i);

        data::ControllerTestVO vo( 12+i );
        Notification note(szCtrKey, &vo);

        controller.executeCommand(note);

        EXPECT_EQ((12+i)*2,vo.result);
    }
}

UNITTEST(control_view)
{
    IController& controller = Controller::getInstance("ControllerTestKey2");

    static data::ControllerTestCommand command;

    for (int i=0;i<10; i++)
    {
        char szCtrKey[64] = { 0 };
        sprintf(szCtrKey,"ControllerTest%d",i);
        controller.registerCommand(szCtrKey, &command);
    }

    for (int i=0; i<10; i++)
    {
        char szCtrKey[64] = { 0 };
        sprintf(szCtrKey,"ControllerTest%d",i);

        data::ControllerTestVO vo( 12+i );
        Notification note(szCtrKey, &vo);

        controller.executeCommand(note);

        EXPECT_EQ((12+i)*2,vo.result);
    }
}

