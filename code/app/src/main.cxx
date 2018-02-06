#include "../include/UI_MainWindow.h"
#include <FL/fl_ask.H>
#include <memory>
#include <iostream>
#include <stdexcept>

int main (int argc, char* argv[])
{
    Fl::scheme ("gtk+");
    Fl::background (0xEC, 0xE9, 0xD8);
    
    std::unique_ptr<EE698K::GUI::UI_MainWindow> ui;
    
    try {

		int X, Y, W, H;
		Fl::screen_xywh (X, Y, W, H);
		
		ui.reset (new EE698K::GUI::UI_MainWindow);
		ui->resize (X, Y, W, H);
		ui->show (argc, argv);
        
        ui->size_range (W, H, W, H);
        
		return Fl::run();
	}
	catch (std::exception& e)
	{
		if (ui) ui->hide();
		fl_alert (
			"%s", e.what());
		std::cerr << e.what() << std::endl;
	}
    
}