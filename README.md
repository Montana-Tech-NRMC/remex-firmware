## HOW TO CLONE THIS REPOSITORY INTO CODE COMPOSER

Step 1: In code compser create a new empty project by clicking on File>New>CCS Project and clicking "Empty Project"

Step 2: Set the project to target the MSP430 FR2355 and name the project Remex

Step 3: Using git initialize a new repository in the folder that was just created using `git init`

Step 4: Set the remote of the git repository to this gitlab repo by using the command `git remote add origin git@gitlab.cs.mtech.edu:ee-projects/nasa-rmc-software-group/motor_board2.0.git`

Step 5: Pull any updates in from the master branch into your repository using `git pull origin master`

## HOW TO USE THE PROJECT

The project is designed to be adaptable to any system on the robot. The different hardware interfaces are designed such that they can be easily used by each system without the need for modification. The rest of this document will demonstrate how to use the code in this repository to develop firmware to control new robotic systems with the Remex board.

### Remex

The `remex.c` and `remex.h` files are the primary files for the user to edit to utilize the board. The two primary functions that should be used are the `init` function and the `loop` function. The `init` function is called once after the board has turned on and has been set to 8MHz processing. The `loop` function is then continuously called afterwards.

The `remex.h` file has also defined the default register map that the I2C master uses to interface with the board. This register map along with the provided I2C slave functions `recieve_cb`, `transmit_cb`, `start_condition_cb`, and `stop_condition_cb` implement a simple I2C register map implementation that will allow an I2C master to read and write data to the slave board. This also allows the board to execute commands from the master device with the `process_cmd` function.

### Hardware IO

There are two kinds of hardware I/O this board can use. First is the QEI encoder interface and the second type is the limit switch interface. The board can support up to two channels of QEI at once, or one channel of QEI and 2 limit switches at the same time. It is important to note that the second channel of QEI uses the same pins as is used for the limit switches so both cannot be initialized at the same time.

#### QEI Interface

The QEI is defined in the `hardwareIO.h` file. There is only one function `init_encoders(int*, int*)` to initialize the QEI hardware interrupts. It takes two parameters. Each parameter is a pointer to an integer which will store the count of clicks. Interrupts in the `hardwareIO.c` will increment and decrement passed in variable. Either of the parameters can be NULL (0L). If either of the parameters is NULL the encoder related to that parameter will not be initialized

#### Limit switches

Limit switches are also defined in the `hardwareIO.h` file. The `init_switches` the function will initialize hardware interrupts to watch the limit switch pins on a HI/LO edge trigger. The function also configures the pins with pull up resistors to make them active low.

```c
	init_switches(void(*switch_a)(int), void(*switch_b)(int));
```

The two parameters the function takes are function pointers that will be invoked when the interrupt for a limit switch is triggered. This allows the user to determine the behavior the board has in response to switches being triggered. If either function is passed in as NULL (0L) then the corresponding switch is not initialized.


### ADC

This board has 3 channels of 12 bit ADC to sample different signals. The first two channels A and B are dedicated to the on board current sensors on the PCB. The third channel, C, is an auxiliary ADC channel that can be used for any purpose. There are two functions to utilize the ADC on the board.

```c
	void setup_ADC(void (*cba)(int), void (*cbb)(int), void (*cbc)(int));
```

The `setup_ADC` function will initialize the hardware to sample on the ADC pins of the board. The function takes 3 function pointers as parameters. When a sample from the ADC has been processed the corresponding function pointer will be called with the value of the sample passed in to the first parameter.

```c
	void read_ADC(int channel);
```

`read_ADC` trigger the ADC to start reading a new sample. When the ADC is finished processing the sample it will fire the appropriate function pointer with the value of the sample. The first parameter of the function determines which of the 3 ADC channels should be sampled. Constants for each channel are provided in the `adc.h` header file.

### PWM

PWM is used to manage the motor controllers that the Remex board is connected to. The Remex board is able to manage two motor controllers at once and therefore has to channels of PWM. All PWM functions can be found in the `pwm.h` header file. 

```c
	void init_PWM_A();
```

`init_PWM_A` will initialize the timer hardware for PWM control. There is a equivalent `init_PWM_B` which will initialize the second channel of PWM.

```c
	void set_PWM_A(const int in);
```

Setting the outputted PWM signal is as simple as calling the `set_PWM_A` function passing an integer between 2000 and 4000. 2000 being full reverse on continuous rotation devices, and minimum angle on positional devices. 4000 is the full forward on continuous rotation devices and maximum angle on positional devices. Any value outside of this range of 2000 to 4000 will be capped to either 4000 if `in > 4000` or 2000 if `in < 2000`. There is an equivalent

### I2C

The Remex board recieves commands over a serial i2c bus connected through the USB mini port. The Remex board has hardware to deal with i2c and this hardware can be initialized with the function `i2c_slave_init`.

```c
void i2c_slave_init(
		void (*stt_cb)(void),
        void (*stp_cb)(void),
        void (*rx_cb)(unsigned char in),
        void (*tx_cb)(unsigned volatile int *out),
        unsigned char slave_addr);
```

`i2c_slave_init` will set up the hardware to behave as an I2C slave with an address of `slave_addr`. The first four parameters of the `i2c_slave_init` function are pointers to functions that will be called at the different points of the I2C protocol. The first pointer `stt_cb` is called when the microcontroller detects the start condition. The `stp_cb` is called when the stop condition is detected. The `rx_cb` is called when the microcontroller recieves a byte of data over the i2c bus and the recieved byte is passed in as the first parameter. When the microcontroller is requested by the master to send data the `tx_cb` is called and a pointer to the byte to send data is provided. Users of the `tx_cb` function will be able to send a byte in the `tx_cb` function by setting `out` equal to the data to be sent.

### Main

The `main.c` should not need to be modified much by the user. The main function will set the internal clock speed of the processor to 8MHz and is responsible for calling the `init` function  and the `loop` function from `remex.h`.
