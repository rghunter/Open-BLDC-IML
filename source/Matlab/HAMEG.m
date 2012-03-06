%HAMEG M-Code for communicating with an instrument.
%
%   This is the machine generated representation of an instrument control
%   session. The instrument control session comprises all the steps you are
%   likely to take when communicating with your instrument. These steps are:
%   
%       1. Create an instrument object
%       2. Connect to the instrument
%       3. Configure properties
%       4. Write and read data
%       5. Disconnect from the instrument
% 
%   To run the instrument control session, type the name of the M-file,
%   HAMEG, at the MATLAB command prompt.
% 
%   The M-file, HAMEG.M must be on your MATLAB PATH. For additional information 
%   on setting your MATLAB PATH, type 'help addpath' at the MATLAB command 
%   prompt.
% 
%   Example:
%       hameg;
% 
%   See also SERIAL, GPIB, TCPIP, UDP, VISA.
% 
 
%   Creation time: 05-Mar-2012 00:15:32

% Find a serial port object.
obj1 = instrfind('Type', 'serial', 'Port', '/dev/ttyUSB0', 'Tag', '');

% Create the serial port object if it does not exist
% otherwise use the object that was found.
if isempty(obj1)
    obj1 = serial('/dev/ttyUSB0');
else
    fclose(obj1);
    obj1 = obj1(1)
end

% Connect to instrument object, obj1.
fopen(obj1);

% Communicating with instrument object, obj1.
data1 = query(obj1, '*IDN?');

fprintf(obj1, 'INST:OUT1');
fprintf(obj1, 'OUTP:GEN ON');


% Disconnect all objects.
fclose(obj1);

% Clean up all objects.
delete(obj1);

