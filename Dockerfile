FROM osrf/ros:foxy-desktop

# ARG GROUP_ID

# RUN addgroup --gid $GROUP_ID dockergroup
# RUN useradd -ms /bin/bash -g dockergroup -G sudo  user
# USER user

RUN apt update && apt install -y libtool
RUN cd home && git clone https://github.com/mrtazz/restclient-cpp.git && cd restclient-cpp && ./autogen.sh && ./configure && make install
RUN rm -rf /home/restclient-cpp/
RUN cp /usr/local/lib/librestclient-cpp.* /usr/lib/
