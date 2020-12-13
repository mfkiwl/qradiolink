Setup and running
-------

- **Notice**: due to an issue present in GNU radio 3.8, it is necessary to perform a workaround step to re-enable digital modes. You need to run the **volk_profile** command, and after it completes, inspect the file **~/.volk/volk_config** in the /home directory. Locate the line starting with **volk_8u_x4_conv_k7_r2_8u** and ensure it ends with  **spiral spiral** like this: **volk_8u_x4_conv_k7_r2_8u spiral spiral**. If it does not not, change it so it looks like this. After performing this step, digital modes should work again. 
- It is recommended to start the application using the command line when running the first few times and look for any error messages output to the console. Some of them can be ignored safely, others are critical. Logging to console is by default enabled.
- **It is not recommended to run qradiolink as root**
- When first run, go to the **Setup** tab first and configure the options, then click Save before starting TX or RX. Without the correct device arguments or the correct antenna selected, the application can crash when enabling RX or TX. This is not something that the application can control and keep functioning properly.
- LimeSDR-mini and LimeNET-Micro devices require a device string like **soapy=0,driver=lime**. If you are using more than one device, use the **SoapySDRUtil --find** command and add the serial number of the device to the device arguments string, like **soapy=0,serial=583A29CED231,driver=lime**
- ADALM-Pluto requires a device argument like **soapy=0,driver=plutosdr**
- GNU radio main DSP blocks are highly optimized (including on embedded ARM platforms) by using the VOLK library. To minimize the CPU resources consumed by QRadioLink it is recommended to run the **volk_profile** utility after GNU radio has been installed. This command only needs to be run when GNU radio or libvolk are upgraded.
- High sample rates, high FPS rates and high FFT sizes all affect the CPU performance adversely. On embedded platforms with low resources, you can disable the spectrum display completely using the FFT checkbox. The FPS value also sets the rate at which the S-meter and constellation display are updated, so reduce it to minimum usable values. If the controls menu is not visible, the S-meter display will not consume CPU resources. Similar for the Constellation display.
- Pulseaudio can be configured for low latency audio by changing settings in /etc/pulse. If you experience interruptions or audio glitches with Pulseaudio, you can try the following workaround: add **tsched=0** to this line in /etc/pulse/default.pa and restart Pulseaudio
<pre>
load-module module-udev-detect tsched=0
</pre>
Alsa may require you to place an **.asoundrc** file in the home directory with contents similar to this:
<pre>
period_time 0
period_size 1024
buffer_size 4096
rate 48000
</pre>

**Important note to Pulseaudio users: please use the Pulseaudio provided sinks and sources in the configuration dropdown, not the default device (or pulse)**

These audio sinks and sources should look like this:
- alsa_input.pci-0000_04_00.6.analog-stereo
- alsa_output.pci-0000_04_00.6.analog-stereo

**The audio should be completely artefact and interruptions free. If you experience either, the Pulseaudio configuration is probably not correct**

- **New**: You may now transmit at all supported sample rates, not just 1 Msps. This feature is still undergoing testing. Transmitting at higher sample rates will use more CPU cycles and be less efficient.
- Filter widths for reception and transmission of the analog modes (FM, SSB, AM) are configurable. To increase or decrease them, drag the margins of the filter box on the spectrum display. For SSB upper sideband, only the upper filter limit can be configured. For SSB lower sideband, only the lower filter limit is configurable.
- You can change the FFT reference display level by hovering with the mouse on the left sidebar where the dB numbers are displayed and dragging this bar up and down. You can also zoom on this axis by hovering with the mouse and using the scroll whell. The levels will be saved in the settings and will remain the same at the next restart. You can also drag the frequency bar below the FFT display to the left and right, and you can zoom in and out with the mouse wheel on both of the number bars. Right clicking the frequency bar resets the frequency zoom to the normal default value.
- VOIP uses [umurmur](https://github.com/umurmur/umurmur) as a server. A version known to work with qradiolink is mirrored at [qradiolink](https://github.com/qradiolink/umurmur)  You can use QRadioLink as a pure VOIP client without using the radio by selecting "Use PTT for VOIP". For radio over IP operation, you need to toggle "Forward radio" to send the digital or analog radio voice to the VOIP server and viceversa. Any voice packets coming from the server will be transmitted directly after transcoding in this case. Full duplex audio from more than one VOIP client at the same time can now be transmitted. The **Mumble** application is now also compatible with QRadioLink. It is recommended to enable **Push To Talk** in Mumble and maximize the network robustness and latency settings. Text messages from Mumble are displayed inside the application, but no action is taken for channel-wide messages. Text messages can also be sent to the current Mumble channel. If remote control is enabled, private Mumble text messages will control the radio.
The Mumble VOIP connection uses the Opus codec at a higher bitrate, so ensure the server can handle bitrates up to 50 kbit/s per client. The VOIP bitrate can be configured in the **Setup** page. On Android phones, the **Plumble** application can be used as a client.
- The VOIP username will be your callsign, plus a number of 4 random characters allowing you to use multiple clients on the same server. The server password is stored in plain text inside the config file. You can use chmod to set this file readable by your user only.
- Remote control via Mumble private text messages requires enabling remote control in settings, and using the Mumble client (either desktop or Android) to send text messages to the QRadioLink username. Text messages sent to the channel will be ignored by the application. Authentication of the user who is sending the commands is not yet implemented.
- Running headless (no graphical user interface) for usage on embedded platforms like the Raspberry Pi or similar boards requires starting QRadioLink from the command line with the **--headless** option; example:
<pre>
$ qradiolink --headless  >> $HOME/.config/qradiolink/qradiolink.log 2>&1
</pre>
When running in headless mode, console log will be disabled by default with the above command. Init scripts for SysV/systemd will be provided at some point to be able to run QRadioLink as a system service. When running headless from CLI, the network command server is started by default listening on the port configured in the settings file (or 4939 if not configured). Headless and remote operation will usually require you to enable VOIP forwarding either in the configuration file or via a command, unless you want to use audio from the machine where QRadioLink is running. CPU consumption can reach 50% at 800 MHz CPU clock for a headless QRadioLink instance connected to the VOIP network and operating as a duplex repeater (depending on mode used).
- The configuration file is located in $HOME/.config/qradiolink/qradiolink.cfg
- The memory channels storage file is located in $HOME/.config/qradiolink/qradiolink_mem.cfg
- Log messages are stored in $HOME/.config/qradiolink/qradiolink.log (this location will likely change in the future)
- After adding a memory channel, you can edit its values by double clicking on a table cell. This may cause the radio to switch to that channel. The settings are not updated instantly, so if you make a change, after you press Enter, switch to another channel and back to get the updates. A button allows you to save channels before the window is closed.  Saving sorted channels is not possible yet. Otherwise, the channels, like the settings, will be stored on exit (if no application crash meanwhile).
- **Before any upgrade**, please make a backup of the $HOME/.config/qradiolink/ directory in case something goes wrong, to avoid losing settings and channels.
- Digital gain can be safely ignored on most devices. It was added as a workaround for the PlutoSDR and is no longer required. Leave it at 5 (half scale) unless you know better.
- In full duplex operation you need to have sufficient isolation between the TX antenna port and the RX antenna port to avoid overloading your input or destroying the LNA stage.
- In half duplex mode the receiver is muted during transmit and the RX gain is minimized. Do not rely on this feature if using a power amplifier, please use a RF switch (antenna switch) with enough isolation, or introduce attenuators in the relay sequence to avoid destroying the receiver LNA.
- The transmitter of the device is active at all times if enabled, even when no samples are being output. Although there is no signal being generated, local oscillator leakage may be present and show up on the spectrum display. This is not a problem usually, unless if you keep a power amplifier connected and enabled at all times. You can use the USB relays to disable it in this case when not transmitting.
- FreeDV modes and PSK modes are very sensitive to amplifier non-linearity. You should not try to use them within a non-linear envelope to avoid signal distortion, splatter or unwanted spectrum components. Digital gain for these modes has been set in such a way to avoid non-linear zone for most devices output stages. If this is not satisfactory, you can use the digital gain setting to increase the digital gain.
- Receive and transmit gains currently operate as described in the gr-osmosdr manual. At lowest settings, the programmable gain attenuator will be set, following with any IF stages if present and finally any LNA stages if present. This behaviour is desirable since there is no point setting the LNA to a higher value than the PGA if the signal power is already above the P1dB point of the LNA stage. Controls for adjusting individual gain stages can also be found on the controls menu. The name of the gain stage will appear as a tooltip.
- Transmit shift can be positive or negative. After changing the value, you need to press **Enter** to put it into effect. Setting the TX shift is not possible while transmitting a signal. Although the shift is stored as Hertz and you can edit this value in the config, the UI will only allow a value in kHz to be entered (e.g. -7600 kHz standard EU UHF repeater shift). To switch to the reverse repeater frequency, use the **Rev** button.
- USB relays using FTDI (FT232) chipsets are used to control RF switches, power amplifiers and filter boards. To determine if your USB relay board is supported, look for a similar line in  the output of lsusb:
<pre>
Bus 002 Device 003: ID 0403:6001 Future Technology Devices International, Ltd FT232 Serial (UART) IC
</pre>
Do note that the identifier digits are the most important: **0403:6001**. 
At the moment, such USB relays can be sourced on Amazon and Ebay and can be identified by the light silver-blue colour of the board. Other types of relays may be supported in the future.
- QRadioLink can control a maximum of 8 relays, and the used relays can be configured in the Setup page. The setting called **relay_sequence** is an 8 bit char bitmask that can take values from 0 to 255 and each bit controls the relay with the bit+1 number, starting with bit 0 and relay number 1. The order in which relays are activated and deactivated during a transmission cycle is always as follows: activation starting with relay 1 to relay 8, deactivation in reverse order (relay 8 to relay 1). A Python script **( ext/ftdi.py )** is included to help you determine the order of relays on the board.
- Video will be displayed in the upper right corner. There is a V4L2 guide in the docs/ directory for troubleshooting camera settings for advanced users in case of issues.
- **New**: starting with version 0.8.5-rc4, the video transmission also has sound, using the Opus audio codec. There is a known issue with a crash caused by gstreamer when the application has the video mode selected at startup, and after a video transmission the user attempts to switch to another mode. As a workaround, only switch to the video mode after starting the TX flowgraph, and switch to another mode before exiting the application. Since the video mode now uses the Qt backend with gstreamer, there are still some small issues around it which may be solved with future Qt versions.
- IP radio modem operation mode requires net administration priviledges (NET_ADMIN capabilities) to be granted to the application. An error message will be output at startup if these priviledges are not present. You can safely ignore this message if you don't need to use the IP modem facility.
<pre>$ sudo setcap "cap_net_raw,cap_net_admin+eip" ./qradiolink</pre>
- VOX mode requires careful setup of system microphone gain to avoid getting stuck on transmit. The VOX activation level can be configured in the **Setup** page.
- Repeater mode requires the radio to operate in **Duplex** mode. Prior to enabling repeater mode, make sure to configure the TX shift (positive or negative). Mixed mode repeat is  possible, so you can operate the receiver on a different mode to the transmitter (FM to Codec2/Opus/FreeDV or viceversa). If radio forwarding is enabled, audio from the repeater will be broadcast to the VOIP network as well. The repeater can now handle mixing of audio incoming from the VOIP network and coming from the radio receiver so it is possible for two or more users on different connected repeaters to speak simultaneously.
- When operating a repeater linked to the VOIP network, you may experience small delays of voice due to transcoding operations, especially for mixed mode repeaters (in addition to network latencies).
- Setting application internal microphone gain above the middle of the scale might cause clipping and distortion of audio, as the system volume also affects what goes to the radio.
- The VOIP volume slider controls the volume of the audio **sent** to the Mumble server.
- Audio recordings are saved in the directory specified in the settings. Audio is recorded in FLAC (free lossless audio compression) format, with audio data only being written to file when there is something being played back on the audio interface. That means that recording while there is silence will not generate file data. The file name corresponds to the time when the recording was started.
- It is now possible to mute self or deafen self from the UI without disconnecting from the VOIP server.
- The S-meter calibration feature is not complete yet, however you can enter in the Setup tab the level (integer value expressed in dBm) of a known signal (e.g. sent by a generator) to correct the reading. Do NOT apply signals with levels above -30 to 0 dBm to the receiver input as this might damage your receiver, depending on hardware. Please note that the RSSI and S-meter values displayed are relative to the current operating mode filter bandwidth, so the FM reading will be different to a SSB reading! Calibration tables support for different bands may be provided in the future.
- The network remote control feature (for headless mode) is work in progress. The network server will listen on the localhost IPv4 network interface and the default control port is 4939. There is no provision for authentication of the user, so use SSH to log in to the remote system and use telnet from there to localhost port 4939. To use the network remote control feature, you can simply use the telnet program or you can create simple Python or shell scripts to automate the commands. The help command will list all the available commands as well as parameters:
<pre>
telnet localhost 4939
qradiolink> help
qradiolink> quit
Bye!
Connection closed by foreign host.
<pre>