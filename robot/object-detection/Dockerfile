FROM ubuntu:18.04

LABEL Description="ROS melodic compiled for python3 in Ubuntu 18.04" Version="1.0"

#
# ========================== General Ubuntu Settings ==========================
#

RUN printf '\n\n Applying Ubuntu Settings.. \n\n'

#
# Define script parameters
#
ARG shell=/bin/bash

# Replace pathToAppDir with the desired container's path 
# to the root of the ros-based application, named <AppID>. 
# As an example, we set <AppID> = SimulationManager
ARG pathToAppDir="/opt/SimulationManager"

# Path to the catkin workspace of the ROS-based application.
ARG ros_ws="${pathToAppDir}/SimulationManager/ROS/catkin_build_ws"

# Use /bin/bash instead of /bin/sh
RUN mv /bin/sh /bin/sh-old && \
ln -s /bin/bash /bin/sh

# Set timezone based on your location
ENV TZ=America/Sao_Paulo
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# install apt-utils, git, python3, pip3
RUN apt-get update -y && apt-get install -y \ 
    apt-utils \
    python3.6 python-pip python3-pip \
    git \
    lsb-release mesa-utils \
    software-properties-common locales x11-apps \
    gedit gedit-plugins nano \
    screen tree \
    sudo ssh synaptic \
    wget curl unzip htop \
    gdb valgrind \
    libcanberra-gtk* \
    xsltproc \
    libgtest-dev \
    iputils-ping iproute2 \
    && rm -rf /var/lib/apt/lists/* \
    && apt-get clean


#
# ========================== ROS Setup ==========================
#

RUN printf '\n\n Installing ROS.. \n\n'

# Install random stuff that is missing in the ros installation
RUN pip3 install -U Cython packaging
RUN pip3 install -U netifaces gnupg empy rospkg numpy
RUN apt-get update -y && apt-get install -y \
    sip-dev \
    pyqt5-dev \
    python3-sip-dev \
    pyqt5-dev-tools \
    python-catkin-pkg \
    && rm -rf /var/lib/apt/lists/* \
    && apt-get clean

# Install python3 equivalent packages for ros
RUN pip3 install pip --upgrade
RUN pip3 install -U scikit-build
RUN pip3 install -U rosdep rosinstall-generator wstool queuelib nose pyros_setup pyyaml catkin-tools rostful opencv-contrib-python defusedxml
#The following python 2 pkg is still required for catkin_make to run at the catkin_ws
RUN pip install -U catkin_pkg

# Install packages to build ros using python 3
RUN apt-get update -y && apt-get install -y \ 
    python3-rosinstall build-essential \
    && rm -rf /var/lib/apt/lists/* \
    && apt-get clean
RUN pip3 uninstall vcstools -y

# Use ONLY this specific version of vcstool
RUN git clone -b mock_server_tar_test https://github.com/tkruse/vcstools.git \
&& pip3 install ./vcstools/

# Install rosdep to deal with dependencies of further ROS packages
RUN rm -rf /etc/ros/rosdep/sources.list.d/* \
&& rosdep init \
&& rosdep update

# Link python 3 dist to site pckgs and Get and compile all melodic pckgs
RUN ln -s /usr/local/lib/python3.6/dist-packages /usr/local/lib/python3.6/site-packages
RUN apt-get update && apt-get install -y python-catkin-pkg python-rosdep &&\
    mkdir ~/catkin_ws && cd ~/catkin_ws \  
    && rosinstall_generator perception --rosdistro melodic --deps --tar > melodic-desktop.rosinstall \
    && wstool init -j8 src melodic-desktop.rosinstall \ 
    && rosdep install --skip-keys python-catkin-pkg-modules --skip-keys python-rosdep-modules --from-paths src --ignore-src --rosdistro melodic -y \
    && export PYTHONPATH=/usr/local/lib/python3.6/dist-packages \
    && ./src/catkin/bin/catkin_make_isolated --install --install-space /opt/ros/melodic -DPYTHON_EXECUTABLE=/usr/bin/python3 -DCMAKE_BUILD_TYPE=Release -j2 \
    && rm -rf /var/lib/apt/lists/* \
    && apt-get clean

# you MUST source the ros env or it will not work -> copy the env to bashrc
RUN echo "source /opt/ros/melodic/setup.sh" >> ~/.bashrc

#
# ========================== Setup the ROS-based Application  ==========================
#

#RUN printf '\n\n Installing SimulationManager.. \n\n'

#Create the <AppID>'s catkin workspace folder for ROS source files
#RUN mkdir -p ${ros_ws}/src

# Copy the contents of the host's existing app into the container's ROS-based app.
# Set and uncomment the following variables for the desired ROS-based application of yours.
#COPY /SimulationManager ${pathToAppDir}
#RUN chmod -R 776 ${pathToAppDir}

# try compiling the ${ros_ws} for the usage of <AppID>
#WORKDIR ${ros_ws}

#Try to issue a catkin_make in the current directory
#RUN sudo apt-get update && sudo apt-get install --only-upgrade python-catkin-pkg
#RUN /bin/bash -c ". /opt/ros/melodic/setup.sh \
#&& rm -rf /devel /build /logs /src/CMakeLists.txt \
#&& cd src \
#&& catkin_init_workspace ${ros_ws}/src\
#&& cd .. \
#&& catkin config -DPYTHON_EXECUTABLE=/usr/bin/python3 -DPYTHON_INCLUDE_DIR=/usr/include/python3.6m -DPYTHON_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython3.6m.so \
#&& catkin config --install \
#&& catkin_make"

# you MUST source the ros env or it will not work -> copy the env to bashrc
#RUN echo "source ${ros_ws}/devel/setup.bash" >> ~/.bashrc
#RUN /bin/bash -c ". ${ros_ws}/devel/setup.bash"

# Edition tools
RUN apt update && apt install -y nano git \
    && rm -rf /var/lib/apt/lists/* \
    && apt-get clean

RUN rm /bin/sh && ln -s /bin/bash /bin/sh

RUN mkdir sensing-ws && mkdir sensing-ws/src && cd sensing-ws/src &&\
    git clone https://github.com/lajoiepy/object-detection.git

RUN cd sensing-ws && source /opt/ros/melodic/setup.bash &&\
    catkin config -DPYTHON_EXECUTABLE=/usr/bin/python3 -DPYTHON_INCLUDE_DIR=/usr/include/python3.6m -DPYTHON_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython3.6m.so &&\
    catkin config --install &&\
    catkin build

#COPY yolov3.weights /sensing-ws/install/share/object_detection/config/darknet_resources/weights/yolov3.weights

RUN cd /sensing-ws/install/share/object_detection/config/darknet_resources/weights/ &&\
    wget https://pjreddie.com/media/files/yolov3.weights

ADD https://api.github.com/repos/lajoiepy/object-detection/git/refs/heads/master version.json

RUN cd sensing-ws && source /opt/ros/melodic/setup.bash &&\   
    cd src/object-detection &&\
    git pull origin master &&\
    catkin build

RUN git config --global user.name "Pierre-Yves Lajoie" &&\
    git config --global user.email "pierre-yves.lajoie@polymtl.ca"

COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]
