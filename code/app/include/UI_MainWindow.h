#ifndef __EE604A_GUI_MAIN_WINDOW__
#define __EE604A_GUI_MAIN_WINDOW__

#include "WC_MainWindow.h"

/*****************************************************************************/

namespace  EE604A {
namespace GUI {

class UI_MainWindow : public WC_MainWindow
{
public:
    UI_MainWindow (void)
    {}
    
private:
    virtual void WOpen_cb (void) override;
    virtual void WQuit_cb (void) override;
    
private:
    virtual void WInpaint_cb (void) override;
    
    virtual void WSmall_cb (void) override;
    virtual void WMedium_cb (void) override;
    
    virtual void WSavePoints_cb (void) override;
    virtual void WLoadPoints_cb (void) override;
};

} } // namespaces EE604A, GUI 

/*****************************************************************************/
/*****************************************************************************/

#endif