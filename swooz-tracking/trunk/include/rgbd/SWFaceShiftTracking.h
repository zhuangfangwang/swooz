/*******************************************************************************
**                                                                            **
**  SWoOz is a software platform written in C++ used for behavioral           **
**  experiments based on interactions between people and robots               **
**  or 3D avatars.                                                            **
**                                                                            **
**  This program is free software: you can redistribute it and/or modify      **
**  it under the terms of the GNU Lesser General Public License as published  **
**  by the Free Software Foundation, either version 3 of the License, or      **
**  (at your option) any later version.                                       **
**                                                                            **
**  This program is distributed in the hope that it will be useful,           **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of            **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             **
**  GNU Lesser General Public License for more details.                       **
**                                                                            **
**  You should have received a copy of the GNU Lesser General Public License  **
**  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.           **
**                                                                            **
** *****************************************************************************
**          Authors: Guillaume Gibert, Florian Lance                          **
**  Website/Contact: http://swooz.free.fr/                                    **
**       Repository: https://github.com/GuillaumeGibert/swooz                 **
********************************************************************************/

/**
 * \file SWFaceShiftTracking.h
 * \brief Defines SWFaceShiftTracking class
 * \author Lance Florian
 * \date 07/01/13
 */


#ifndef _SWFACESHIFTTRACKING_
#define _SWFACESHIFTTRACKING_

// SWOOZ
#include "SWExceptions.h"

// FACESHIFT
#include "rgbd/faceshift/fsbinarystream.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <Windows.h>

// YARP
#include <yarp/dev/all.h>
#include <yarp/os/all.h>
#include <yarp/sig/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/ResourceFinder.h>


/**
 * \class SWFaceShiftTracking
 * \brief This module sends faceShift data (head,gaze) on two yarp ports
 *
 * Bottles contents :
 *
 * Head :
 *
 *  device id : FACESHIFT_LIB  / get(0).asInt()
 *
 *   translation
 *       Tx / get(1).asDouble()
 *       Ty / get(2).asDouble()
 *       Tz / get(3).asDouble()
 *   rotation (quaternions)
 *       Rx / get(4).asDouble()
 *       Ry / get(5).asDouble()
 *       Rz / get(6).asDouble()
 *
 * Gaze :
 *
 *  device id : FACESHIFT_LIB  / get(0).asInt()
 *
 *  left eye
 *      pitch / get(1).asDouble()
 *      yaw   / get(2).asDouble()
 *
 *  right eye
 *      pitch / get(3).asDouble()
 *      yaw   / get(4).asDouble()
 *
 */
class SWFaceShiftTracking : public yarp::os::RFModule
{
    public :

        /**
         * \brief default constructor of SWFaceShiftTracking
         */
        SWFaceShiftTracking();

        /**
         * \brief Init configuration values with the config file
         * \param [in] oRf : icub resource config file
         * \return true if configuration successful
         */
        bool configure(yarp::os::ResourceFinder &oRf);


        /**
         * \brief Return the init state faceShift.
         * \return true if faceShift has been initialized
         */
        bool isFaceShiftInitialized() const;

    private :

        /**
         * \brief Close connections.
         * \return true (Module heritage)
         */
        bool close();

        /**
         * \brief Interrupt the module.
         * \return true (Module heritage)
         */
        bool interruptModule();

        /**
         * \brief Init faceShift device
         */
        void initFaceShift();

        /**
         * \brief Called periodically every getPeriod() seconds
         * \return true
         */
        bool updateModule();

        /**
         * \brief Retrieve the update function call period of the module.
         * \return the period
         */
        double getPeriod();

    private :

        bool m_bIsFaceShiftInitialized;         /**< is faceshift initialized ? */
        int m_i32Fps;                           /**< refresh rate of updateModule calling */
        std::string m_sHeadTrackingPortName;    /**< yarp head tracking port name */
        std::string m_sGazeTrackingPortName;    /**< yarp gaze tracking port name */
        std::string m_sFaceTrackingPortName;    /**< yarp face tracking port name */
        yarp::os::BufferedPort<yarp::os::Bottle> m_oHeadTrackingPort;   /**< yarp head tracking port */
        yarp::os::BufferedPort<yarp::os::Bottle> m_oGazeTrackingPort;   /**< yarp gaze tracking port */
        yarp::os::BufferedPort<yarp::os::Bottle> m_oFaceTrackingPort;   /**< yarp face tracking port */

        // faceshift
        SOCKET ConnectSocket;                   /**< faceshift socket */
        int iResult;                            /**< result reception message */
        int recvbuflen;                         /**< reception buffer */
        fs::fsBinaryStream parserIn;            /**< parser input */
        fs::fsMsgPtr msg;                       /**< message input */
};


#endif
