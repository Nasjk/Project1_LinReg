/********************************************************************************
 * @brief Implementing a linear regression model and deploying it on an 
 *        Arduino Uno to predict room temperature using linear regression. 
 *        Written in C++.
 ********************************************************************************/
#include <drivers.hpp> 
#include <lin_reg.hpp>

using namespace yrgo::driver;
using namespace yrgo::container;

/********************************************************************************
 * @brief Devices and models used in the embedded system.
 *
 * @param model
 *        Linear regression model for predicting the room temperature.
 * @param button1
 *        Button used to toggle the temperature.
 * @param timer0
 *        Timer used to reduced the effect of contact bounces when pressing
 *        the button.
 * @param timer1
 *        Timer used to toggle the temperature every 60s when enabled.
 ********************************************************************************/
static yrgo::LinReg model{};
static GPIO button1{13, GPIO::Direction::kInputPullup};
static Timer timer0{Timer::Circuit::k0, 300};
static Timer timer1{Timer::Circuit::k1, 60000};

/********************************************************************************
 * @brief Read the analog voltage from pin A2 and convert it to a voltage in the range of 
 *        0 to 5 volts.
 *
 *			 Use the pre-trained linear regression model ('model') to 
 *		    predict the temperature based on the voltage reading.
 *
 *			 Print the predicted temperature to the serial monitor, rounded to the nearest integer.
 ********************************************************************************/

namespace {

void PredictTemp(void){
	const auto uin= adc::Read(adc::Pin::A2) / 1023.0 * 5; 
	const auto predicted_temp = model.Predict(uin);
	serial::Printf("Temp: %d\n", utils::Round(predicted_temp));
}

/********************************************************************************
 * @brief Callback routine called when button1 is pressed or released.
 *        Every time button1 is pressed, the temperature is predicted and the
 *        60 second timer is restated.
 *        Pin change interrupts are disabled for 300 ms on the button's I/O port
 *        to reduce the effects of contact bounces.
 ********************************************************************************/
void ButtonCallback(void) 
{
    button1.DisableInterruptsOnIoPort();
    timer0.Start();

	if (button1.Read()) 
	{
		PredictTemp(); 
		timer1.Restart();
	}
}

/********************************************************************************
 * @brief Enabled pin change interrupts on the button's I/O port 300 ms after
 *        press or release to reduce the effects of contact bounces.
 ********************************************************************************/
void Timer0Callback(void) {
    if (timer0.Elapsed()) {
        timer0.Stop();
	    button1.EnableInterruptsOnIoPort();
	}
}

/********************************************************************************
 * @brief Toggles led1 when timer1 elapsed, which is every 60 s when enabled.
 ********************************************************************************/
void Timer1Callback(void) {
    if (timer1.Elapsed()) {
        PredictTemp();
    }
}

/********************************************************************************
 * @brief Sets callback routines, enabled pin change interrupt on button1 and
 *        enables the watchdog timer in system reset mode.
 ********************************************************************************/
inline void Setup(void) {

	const Vector<double> inputs{{0.0, 1.0, 2.0, 3.0, 4.0}};
	const Vector<double> outputs{{-50.0, 50.0, 150.0, 250.0, 350.0}};
	model.LoadTrainingData(inputs, outputs);
	model.Train(1000);
	
	serial::Init();
	PredictTemp();
	timer1.Start();

	button1.SetCallbackRoutine(ButtonCallback);
	timer0.SetCallback(Timer0Callback);
   timer1.SetCallback(Timer1Callback);
	
	button1.EnableInterrupt();
	watchdog::Init(watchdog::Timeout::k1024ms);
	watchdog::EnableSystemReset();
}

} /* namespace */

/********************************************************************************
 * @brief Perform a setup of the system, then running the program as long as
 *        voltage is supplied. The hardware is interrupt controlled, hence the
 *        while loop is almost empty. If the program gets stuck anywhere, the
 *        watchdog timer won't be reset in time and the program will then restart.
 ********************************************************************************/
int main(void)
{
    Setup();

    while (1) 
    {
	    watchdog::Reset();
    }
	return 0;
}

