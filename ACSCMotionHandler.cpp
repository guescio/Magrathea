#include "ACSCMotionHandler.h"
#include <QtMessageHandler>

///// Understand why the simulator freeze sometimes....

//******************************************
ACSCMotionHandler::ACSCMotionHandler() :
    gantryConnected(false),
    xAxisEnabled(false),
    yAxisEnabled(false),
    zAxisEnabled(false),
    uAxisEnabled(false)
    {
    gantry = NULL;

    //    xAxis = (AXISMASK)AXISMASK_02;
    //    xIndex = (AXISINDEX)AXISINDEX_02;

    //    yAxis = (AXISMASK)AXISMASK_00; // AXISMASK_01 is YY, the y-axis slave
    //    yIndex = AXISINDEX_00;

    //    zAxis = (AXISMASK)AXISMASK_03;
    //    zIndex = AXISINDEX_03;

    //    uAxis = (AXISMASK)AXISMASK_04;
    //    uIndex = AXISINDEX_04;

    //    allAxes = (AXISMASK)(AXISMASK_02 | AXISMASK_00 | AXISMASK_03 | AXISMASK_04);
    //    xyAxes = (AXISMASK)(AXISMASK_02 | AXISMASK_00);
}

//******************************************
ACSCMotionHandler::~ACSCMotionHandler() {
    if (gantry != NULL) {
        if(acsc_DisableAll(gantry,ACSC_SYNCHRONOUS) == 0 )
            qWarning("Error stopping all axes: %d ",acsc_GetLastError());
        else
            qWarning("stopped all axes");
        if(acsc_CloseSimulator() == 0 )
            qWarning("Closing simulator error: %d ",acsc_GetLastError());
        else
            qWarning("closed simulator");
        ///////////////////////////////////////////////
        //connection functions: see: SPiiPlus C Library Reference Programmer Guide.pdf
        //if(!acsc_CloseComm(gantry));
        //  qWarning("Closing gantry connection error: %d ",acsc_GetLastError());
        //else
        //  qWarning("Connection closed");
        ///////////////////////////////////////////////
    }
}

//******************************************
// connect to the gantry

//------------------------------------------
bool ACSCMotionHandler::connectGantry(bool flag)
{
    if (flag) {
        qInfo("Closing pending connections.");
        ACSC_CONNECTION_DESC connection_descriprtions[100];
        int NumberOfConnections = 0;
        if(acsc_GetConnectionsList(connection_descriprtions,100,&NumberOfConnections) == 0 )
            qInfo("Error getting number of connections: %d ",acsc_GetLastError());
        for(int i =0; i<NumberOfConnections;i++){
            QString str(connection_descriprtions[i].Application);
            if(acsc_TerminateConnection (& (connection_descriprtions[i])))
                qInfo("Error closing application %s connection: %d ",str.toLocal8Bit().constData(),acsc_GetLastError());
            else
                qInfo("Application %s connection closed.",str.toLocal8Bit().constData());
        }
        //end of closing pending connections

        //add code for real gantry in comment
        qInfo("connecting gantry...");
        gantry = acsc_OpenCommSimulator(); //connect gantry here
        ///////////////////////////////////////////////
        //connection functions: see: SPiiPlus C Library Reference Programmer Guide.pdf
        //gantry = acsc_OpenCommSerial(1,ACSC_AUTO);
        //gantry = acsc_OpenCommEthernetTCP(<address>,<port>); //see manual for reference
        //gantry = acsc_OpenCommEthernetUDP(<address>,<port>); //see manual for reference
        ///////////////////////////////////////////////
        if(gantry == ACSC_INVALID){ //connect gantry here
            qWarning("Error init Gantry: %d ",acsc_GetLastError());
            qWarning("could not connect gantry");
            gantryConnected=false;
            return false;
        }else{
            qInfo("gantry connected");
            gantryConnected=true;
            return true;
        }
    } else {
        qInfo("disconnecting gantry...");
        if (acsc_CloseSimulator() != 0 ) { //disconnect gantry here
            qInfo("gantry disconnected");
            gantryConnected=false;
            return true;
        }else {
            qWarning("could not disconnect gantry");
            return false;
        }
    }
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::disconnectGantry()
{
    return connectGantry(false);
}

//------------------------------------------
bool ACSCMotionHandler::stop(){
    qInfo("stopping...");
    int Axes[5] = {X_axis,Y_axis,
                         Z_axis,U_axis,-1};
    if (acsc_BreakM(gantry,Axes,ACSC_SYNCHRONOUS) != 0) { //stop here
        qInfo("stop");
        return true;
    } else {
        qWarning("could not stop");
        qInfo("Error : %d ",acsc_GetLastError());
        return false;
    }
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::acknowledgeMotionFaultGantry()
{
    qInfo("resetting errors...");
    if (true) { //acknowledge and clear axes faults
        qInfo("errors reset");
        return true;
    } else {
        qWarning("could not reset errors");
        return false;
    }
    return true;}


//------------------------------------------
bool ACSCMotionHandler::enableAxes(bool flag)
{
    if (flag) {
        qInfo("enabling axes...");
        int Axes[5] = {X_axis,Y_axis,
                             Z_axis,U_axis,-1};
        if (acsc_EnableM(gantry,Axes,ACSC_SYNCHRONOUS) != 0 ) { //enable all axes here
            qInfo("axes enabled");
            return true;
        } else {
            qInfo("Error initiating axes: %d ",acsc_GetLastError());
            qWarning("could not enable axes");
            return false;
        }
    } else {
        qInfo("disabling axes...");
        if (acsc_DisableAll(gantry,ACSC_SYNCHRONOUS) != 0 ) { //disable all axes here
            qInfo("axes disabled");
            return true;
        } else {
            qWarning("Error stopping all axes: %d ",acsc_GetLastError());
            qWarning("could not disable axes");
            return false;
        }
    }
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::enableXAxis(bool flag)
{
    if (flag) {
        qInfo("enabling x axis...");
        if (acsc_Enable(gantry,X_axis,ACSC_SYNCHRONOUS) != 0 ) { //enable x axis here
            qInfo("x axis enabled");
            xAxisEnabled=true;
            return true;
        } else {
            qInfo("Error initiating axis 0: %d ",acsc_GetLastError());
            qInfo("could not enable x axis");
            return false;
        }
    } else {
        qInfo("disabling x axis...");
        if (true) { //disable x axis here
            //add axes disabling function
            qInfo("x axis disabled");
            xAxisEnabled=false;
            return true;
        } else {
            qInfo("could not disable x axis");
            return false;
        }
    }
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::enableYAxis(bool flag)
{
    if (flag) {
        qInfo("enabling y axis...");
        if (acsc_Enable(gantry,Y_axis,ACSC_SYNCHRONOUS) != 0 ) { //enable y axis here
            qInfo("y axis enabled");
            yAxisEnabled=true;
            return true;
        } else {
            qInfo("Error initiating axis 1: %d ",acsc_GetLastError());
            qInfo("could not enable y axis");
            return false;
        }
    } else {
        qInfo("disabling y axis...");
        if (true) { //disable y axis here
            //add axes disabling function
            qInfo("y axis disabled");
            yAxisEnabled=false;
            return true;
        } else {
            qInfo("could not disable y axis");
            return false;
        }
    }
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::enableZAxis(bool flag)
{
    if (flag) {
        qInfo("enabling z axis...");
        if (acsc_Enable(gantry,Z_axis,ACSC_SYNCHRONOUS) != 0 ) { //enable z axis here
            qInfo("z axis enabled");
            zAxisEnabled=true;
            return true;
        } else {
            qInfo("Error initiating axis 2: %d ",acsc_GetLastError());
            qInfo("could not enable z axis");
            return false;
        }
    } else {
        qInfo("disabling z axis...");
        if (true) { //disable z axis here
            //add axes disabling function
            qInfo("z axis disabled");
            zAxisEnabled=false;
            return true;
        } else {
            qInfo("could not disable z axis");
            return false;
        }
    }
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::enableUAxis(bool flag)
{
    if (flag) {
        qInfo("enabling u axis...");
        if (acsc_Enable(gantry,U_axis,ACSC_SYNCHRONOUS) != 0 ) { //enable u axis here
            qInfo("u axis enabled");
            uAxisEnabled=true;
            return true;
        } else {
            qInfo("Error initiating axis 3: %d ",acsc_GetLastError());
            qInfo("could not enable u axis");
            return false;
        }
    } else {
        qInfo("disabling u axis...");
        if (true) { //disable u axis here
            //add axes disabling function
            qInfo("u axis disabled");
            uAxisEnabled=false;
            return true;
        } else {
            qInfo("could not disable u axis");
            return false;
        }
    }
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::disableAxes()
{
    return enableAxes(false);
}

//------------------------------------------
bool ACSCMotionHandler::disableXAxis()
{
    return enableXAxis(false);
}

//------------------------------------------
bool ACSCMotionHandler::disableYAxis()
{
    return enableYAxis(false);
}

//------------------------------------------
bool ACSCMotionHandler::disableZAxis()
{
    return enableZAxis(false);
}

//------------------------------------------
bool ACSCMotionHandler::disableUAxis()
{
    return enableUAxis(false);
}

//******************************************
// home axes

//------------------------------------------
bool ACSCMotionHandler::home() {
    qInfo("homing axes...");
    homeX();
    homeY();
    homeZ();
    homeU();
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::homeX() {
    qInfo("homing x axis...");
    moveXTo(Home_coord[0],default_speed);
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::homeY() {
    qInfo("homing y axis...");
    moveYTo(Home_coord[1],default_speed);
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::homeZ() {
    qInfo("homing z axis...");
    moveZTo(Home_coord[2],default_speed);
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::homeU() {
    qInfo("homing u axis...");
    moveUTo(Home_coord[3],default_angular_speed);
    return true;
}

//******************************************
// absolute motion
// NOTE units in mm, mm/s and deg/s

//------------------------------------------
bool ACSCMotionHandler::moveTo(double x, double y, double z, double speed)
{
    qInfo("moving to (%.3f mm, %.3f mm, %.3f mm) at %.3f mm/s speed...", x, y, z, speed);
    moveXTo(x,speed);
    moveYTo(y,speed);
    moveZTo(z,speed);
    qInfo("...");
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::moveXTo(double x, double speed) {
    qInfo("moving x axis to %.3f mm at %.3f mm/s speed", x, speed);
    if(acsc_SetVelocity(gantry,X_axis,speed,ACSC_SYNCHRONOUS) == 0)
        qWarning("Error gantry, setting speed X axis: %d ",acsc_GetLastError());
    if (acsc_ToPoint(gantry,0,X_axis,x,ACSC_SYNCHRONOUS) != 0) { //move to destination here
        if(acsc_WaitMotionEnd(gantry,X_axis,TimeOut) == 0)
            qWarning("Error gantry, waiting motion X axis to end: %d ",acsc_GetLastError());//wait
        qInfo("moved x axis to destination");
        emit updatePositions_s();
        return true;
    } else {
        qWarning("Error gantry, motion X axis: %d ",acsc_GetLastError());
        qWarning("could not move x axis to destination");
        emit updatePositions_s();
        return false;
    }
    emit updatePositions_s();
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::moveYTo(double y, double speed) {
    qInfo("moving y axis to %.3f mm at %.3f mm/s speed", y, speed);
    if(acsc_SetVelocity(gantry,Y_axis,speed,ACSC_SYNCHRONOUS) == 0)
        qWarning("Error gantry, setting speed Y axis: %d ",acsc_GetLastError());
    if (acsc_ToPoint(gantry,0,Y_axis,y,ACSC_SYNCHRONOUS) != 0) { //move to destination here
        if(acsc_WaitMotionEnd(gantry,Y_axis,TimeOut) == 0)
            qWarning("Error gantry, waiting motion Y axis to end: %d ",acsc_GetLastError());//wait
        qInfo("moved Y axis to destination");
        emit updatePositions_s();
        return true;
    } else {
        qWarning("Error gantry, motion Y axis: %d ",acsc_GetLastError());
        qWarning("could not move Y axis to destination");
        emit updatePositions_s();
        return false;
    }
    emit updatePositions_s();
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::moveZTo(double z, double speed) {
    qInfo("moving z axis to %.3f mm at %.3f mm/s speed", z, speed);
    if(acsc_SetVelocity(gantry,Z_axis,speed,ACSC_SYNCHRONOUS) == 0)
        qWarning("Error gantry, setting speed Z axis: %d ",acsc_GetLastError());
    if (acsc_ToPoint(gantry,0,Z_axis,z,ACSC_SYNCHRONOUS) != 0) { //move to destination here
        if(acsc_WaitMotionEnd(gantry,Z_axis,TimeOut) == 0)
            qWarning("Error gantry, waiting motion Z axis to end: %d ",acsc_GetLastError());//wait
        qInfo("moved Z axis to destination");
        emit updatePositions_s();
        return true;
    } else {
        qWarning("Error gantry, motion Z axis: %d ",acsc_GetLastError());
        qWarning("could not move Z axis to destination");
        emit updatePositions_s();
        return false;
    }
    emit updatePositions_s();
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::moveUTo(double u, double speed) {
    qInfo("moving U axis to %.3f <set angle units> at %.3f <set angle units>/s speed", u, speed);
    if(acsc_SetVelocity(gantry,U_axis,speed,ACSC_SYNCHRONOUS) == 0)
        qWarning("Error gantry, setting speed U axis: %d ",acsc_GetLastError());
    if (acsc_ToPoint(gantry,0,U_axis,u,ACSC_SYNCHRONOUS) != 0) { //move to destination here
        if(acsc_WaitMotionEnd(gantry,U_axis,TimeOut) == 0)
            qWarning("Error gantry, waiting motion U axis to end: %d ",acsc_GetLastError());//wait
        qInfo("moved U axis to destination");
        emit updatePositions_s();
        return true;
    } else {
        qWarning("Error gantry, motion U axis: %d ",acsc_GetLastError());
        qWarning("could not move U axis to destination");
        emit updatePositions_s();
        return false;
    }
    emit updatePositions_s();
    return true;
}

//******************************************
// step  motion
// NOTE units in mm, mm/s and deg/s

//------------------------------------------
bool ACSCMotionHandler::moveBy(double x, double y, double z, double speed)
{
    qInfo("moving to (%.3f mm, %.3f mm, %.3f mm) at %.3f mm/s speed...", x, y, z, speed);
    moveXTo(x,speed);
    moveYTo(y,speed);
    moveZTo(z,speed);
    qInfo("...");
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::moveXBy(double x, double speed) {
    qInfo("moving x axis to %.3f mm at %.3f mm/s speed", x, speed);
    if(acsc_SetVelocity(gantry,X_axis,speed,ACSC_SYNCHRONOUS) == 0)
        qWarning("Error gantry, setting speed X axis: %d ",acsc_GetLastError());
    if (acsc_ToPoint(gantry,ACSC_AMF_RELATIVE,X_axis,x,ACSC_SYNCHRONOUS) != 0) { //move to destination here
        if(acsc_WaitMotionEnd(gantry,X_axis,TimeOut) == 0)
            qWarning("Error gantry, waiting motion X axis to end: %d ",acsc_GetLastError());//wait
        qInfo("moved x axis to destination");
        emit updatePositions_s();
        return true;
    } else {
        qWarning("Error gantry, motion X axis: %d ",acsc_GetLastError());
        qWarning("could not move x axis to destination");
        emit updatePositions_s();
        return false;
    }
    emit updatePositions_s();
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::moveYBy(double y, double speed) {
    qInfo("moving y axis to %.3f mm at %.3f mm/s speed", y, speed);
    if(acsc_SetVelocity(gantry,Y_axis,speed,ACSC_SYNCHRONOUS) == 0)
        qWarning("Error gantry, setting speed Y axis: %d ",acsc_GetLastError());
    if (acsc_ToPoint(gantry,ACSC_AMF_RELATIVE,Y_axis,y,ACSC_SYNCHRONOUS) != 0) { //move to destination here
        if(acsc_WaitMotionEnd(gantry,Y_axis,TimeOut) == 0)
            qWarning("Error gantry, waiting motion Y axis to end: %d ",acsc_GetLastError());//wait
        qInfo("moved Y axis to destination");
        emit updatePositions_s();
        return true;
    } else {
        qWarning("Error gantry, motion Y axis: %d ",acsc_GetLastError());
        qWarning("could not move Y axis to destination");
        emit updatePositions_s();
        return false;
    }
    emit updatePositions_s();
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::moveZBy(double z, double speed) {
    qInfo("moving z axis to %.3f mm at %.3f mm/s speed", z, speed);
    if(acsc_SetVelocity(gantry,Z_axis,speed,ACSC_SYNCHRONOUS) == 0)
        qWarning("Error gantry, setting speed Z axis: %d ",acsc_GetLastError());
    if (acsc_ToPoint(gantry,ACSC_AMF_RELATIVE,Z_axis,z,ACSC_SYNCHRONOUS) != 0) { //move to destination here
        if(acsc_WaitMotionEnd(gantry,Z_axis,TimeOut) == 0)
            qWarning("Error gantry, waiting motion Z axis to end: %d ",acsc_GetLastError());//wait
        qInfo("moved Z axis to destination");
        emit updatePositions_s();
        return true;
    } else {
        qWarning("Error gantry, motion Z axis: %d ",acsc_GetLastError());
        qWarning("could not move Z axis to destination");
        emit updatePositions_s();
        return false;
    }
    emit updatePositions_s();
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::moveUBy(double u, double speed) {
    qInfo("moving U axis to %.3f <set angle units> at %.3f <set angle units>/s speed", u, speed);
    if(acsc_SetVelocity(gantry,U_axis,speed,ACSC_SYNCHRONOUS) == 0)
        qWarning("Error gantry, setting speed U axis: %d ",acsc_GetLastError());
    if (acsc_ToPoint(gantry,0,U_axis,u,ACSC_SYNCHRONOUS) != 0) { //move to destination here
        if(acsc_WaitMotionEnd(gantry,U_axis,TimeOut) == 0)
            qWarning("Error gantry, waiting motion U axis to end: %d ",acsc_GetLastError());//wait
        qInfo("moved U axis to destination");
        emit updatePositions_s();
        return true;
    } else {
        qWarning("Error gantry, motion U axis: %d ",acsc_GetLastError());
        qWarning("could not move U axis to destination");
        emit updatePositions_s();
        return false;
    }
    emit updatePositions_s();
    return true;
}


//******************************************
// free run
// NOTE units in mm, mm/s and deg/s

//------------------------------------------
bool ACSCMotionHandler::runX(double direction, double speed)
{
    qWarning("Could not free run along X axis");
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::endRunX()
{
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::runY(double direction, double speed)
{
    qWarning("Could not free run along Y axis");
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::endRunY()
{
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::runZ(double direction, double speed)
{
    qWarning("Could not free run along Z axis");
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::endRunZ()
{
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::runU(double direction, double speed)
{
    qWarning("Could not free run along U axis");
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::endRunU()
{
    return true;
}

//******************************************
//gantry current position
std::vector<double> ACSCMotionHandler::whereAmI() {
    std::vector<double> position = {-99990.0,-99990.0,-99990.0,-99990.0};
    double position_tmp = -99999.9;
    if(!gantryConnected)
        return position;
    if(acsc_GetFPosition(gantry,X_axis,&position_tmp,ACSC_SYNCHRONOUS) == 0)
        qWarning("Error get position X axis: %d ",acsc_GetLastError());
    position[0] = position_tmp;

    if(acsc_GetFPosition(gantry,Y_axis,&position_tmp,ACSC_SYNCHRONOUS) == 0)
        qWarning("Error get position Y axis: %d ",acsc_GetLastError());
    position[1] = position_tmp;

    if(acsc_GetFPosition(gantry,Z_axis,&position_tmp,ACSC_SYNCHRONOUS) == 0)
        qWarning("Error get position Z axis: %d ",acsc_GetLastError());
    position[2] = position_tmp;

    if(acsc_GetFPosition(gantry,U_axis,&position_tmp,ACSC_SYNCHRONOUS) == 0)
        qWarning("Error get position U axis: %d ",acsc_GetLastError());
    position[3] = position_tmp;

    return position;
}
