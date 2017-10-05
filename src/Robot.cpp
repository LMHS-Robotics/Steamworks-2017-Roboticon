#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>
#include <time.h>

#include <WPIlib.h>
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <DoubleSolenoid.h>
#include <CameraServer.h>
#include <Compressor.h>
#include <Joystick.h>
#include <ctrlib/CANTalon.h>

class Robot: public frc::IterativeRobot {
public:
	void RobotInit() {
		CameraServer::GetInstance()->StartAutomaticCapture();//cam0
		CameraServer::GetInstance()->StartAutomaticCapture();//cam1
		jL = new Joystick(0);
		leftGearShift = new DoubleSolenoid(0, 1);
		rightGearShift = new DoubleSolenoid(2, 3);
		grabSolenoidL = new DoubleSolenoid(4, 5);
		grabSolenoidR = new DoubleSolenoid(6, 7);
		fL = new CANTalon(12);
		fR = new CANTalon(14);
		bL = new CANTalon(13);
		bR = new CANTalon(15);
		liftMotor = new CANTalon(7);
		c = new Compressor(0);
		shiftToggle = false;
		timer = 0;
	}

	void DisabledPeriodic() override{

	}

	void AutonomousInit() override {
		timer = 0;
	}

	void AutonomousPeriodic() {

		while(timer < 200){//100 loops is roughly 2 seconds

			float left = 0.5;//half speed
			float right = -0.5;

			fL->Set(left);
			bL->Set(left);
			fR->Set(right);
			bR->Set(right);

		}


	}

	void TeleopInit() {
		c->SetClosedLoopControl(true);// this needs to be changed so the compressor turns on when the robot does, so you start the game with air pressurized
		leftGearShift->Set(DoubleSolenoid::Value::kForward);
		rightGearShift->Set(DoubleSolenoid::Value::kForward);
	}

	void TeleopPeriodic() {
	
		//tank drive with two joystick axis on ONE joystick (ps3 for example)
		if(fabs(jL->GetRawAxis(2)) > 0.1 || fabs(jL->GetRawAxis(5)) > 0.1){
				float left = jL->GetRawAxis(2);
				float right = -1 * jL->GetRawAxis(5);// must negate the right one so both get some values, as one will be negative and other will be postive before fixing
				fL->Set(left);
				bL->Set(left);
				fR->Set(right);
				bR->Set(right);
		}
	

		//Shift the gears!
		/*while(jL->GetRawButton(3)){//dont use while loop, remember teleop peridoc function IS a loop. all this code is looped continiously while teleop is enabled
			if(leftGearShift->Get() == DoubleSolenoid::Value::kForward){
				leftGearShift->Set(DoubleSolenoid::Value::kReverse);
			} else {
				leftGearShift->Set(DoubleSolenoid::Value::kForward);
			}

			if(rightGearShift->Get() == DoubleSolenoid::Value::kForward){
				rightGearShift->Set(DoubleSolenoid::Value::kReverse);
			} else {
				rightGearShift->Set(DoubleSolenoid::Value::kForward);
			}
		//so idk what you were trying to do, i think a toggle? check this out:
		}*/
		
		//shift it, i can make it a toggle if thats better
		
		if(jL->GetRawButton(5)){
			
			if(shiftToggle){
				if(leftGearShift->Get() == DoubleSolenoid::Value::kForward){
					leftGearShift->Set(DoubleSolenoid::Value::kReverse);
				}
			
				if(rightGearShift->Get() == DoubleSolenoid::Value::kForward){
					rightGearShift->Set(DoubleSolenoid::Value::kReverse);
				}
				shiftToggle = false;
			}else{
				if(leftGearShift->Get() == DoubleSolenoid::Value::kReverse){
					leftGearShift->Set(DoubleSolenoid::Value::kForward);
				}

				if(rightGearShift->Get() == DoubleSolenoid::Value::kReverse){
					rightGearShift->Set(DoubleSolenoid::Value::kForward);
				}
			}
			shiftToggle = true;
		}
		
		//Open/Close the gear grabber
		if(jL->GetRawButton(6)){
			grabSolenoidL->Set(DoubleSolenoid::Value::kForward);
			grabSolenoidR->Set(DoubleSolenoid::Value::kForward);
		} else if (!jL->GetRawButton(6)){
			grabSolenoidL->Set(DoubleSolenoid::Value::kReverse);
			grabSolenoidR->Set(DoubleSolenoid::Value::kReverse);
		}

		if(jL->GetRawButton(1)){
			liftMotor->Set(1);
		} else {
			liftMotor->Set(0);
		}
	}

	void TestPeriodic() {
		lw->Run();
	}

private:
	frc::LiveWindow* lw = LiveWindow::GetInstance();
	//Joystick
	Joystick *jL;
	//Buttons
	bool shiftToggle;
	//Solenoids
	DoubleSolenoid *leftGearShift;
	DoubleSolenoid *rightGearShift;
	DoubleSolenoid *grabSolenoidL;
	DoubleSolenoid *grabSolenoidR;
	//Motors
	CANTalon *fL;
	CANTalon *fR;
	CANTalon *bL;
	CANTalon *bR;
	CANTalon *liftMotor;
	Compressor *c;
	int timer;
};

START_ROBOT_CLASS(Robot)
