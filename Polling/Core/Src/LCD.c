#include "LCD.h"

/* GLOBAL VARIABLES ---------------------------------------------*/

uint8_t current_page = 1;

// All the rows of data that will be on a single page

		// ***TEMPORARY: DISPLAY THE DATA FROM TOP DOWN OF THE DisplayDataArray[]

DisplayVar DisplayLayoutPage1[] = {
		{ "BATT VOLT", "kV", 9, 3, 10, 13, 0 },
		{ "BATT TEMP", "C", 9, 3, 10, 13, 1 },
		{ "SPEED", "KMH", 5, 3, 6, 9, 2 },
		{ "SOC", "%", 9, 3, 10, 13, 3 }
};

DisplayVar DisplayLayoutPage2[] = {
		{ "MOTOR TEMP", "C", 10, 3, 11, 14, 0 },
		{ "MOTOR VEL", "RPM", 9, 3, 10, 13, 1 },
		{ "BMS FAULT", "", 9, 3, 10, 13, 2 },
		{ "CRUISE ON", "", 9, 3, 10, 13, 3 }
};

/*
DisplayVar DisplayLayoutPage1[] = {
		{ "BATT VOLT", "V", 9, 3, 10, 13, 0 },
		{ "BATT TEMP", "C", 9, 3, 10, 13, 1 },
		{ "SPEED", "KMH", 5, 3, 6, 9, 2 },
		{ "SOC", "%", 9, 3, 10, 13, 3 }
};

DisplayVar DisplayLayoutPage2[] = {
		{ "BATT VOLT", "V", 9, 3, 10, 13, 0 },
		{ "BATT TEMP", "C", 9, 3, 10, 13, 1 },
		{ "SPEED", "KMH", 5, 3, 6, 9, 2 },
		{ "SOC", "%", 9, 3, 10, 13, 3 }
};
*/

// All of the unparsed CAN data, updated whenever new CAN data is received
// And, all of the parsed data
DisplayData DisplayDataArray[] = {

			// ***TEMPORARY: STORE ALL CAN IDs THAT WILL BE DISPLAYED, IN NUMBERICAL ORDER

		{ 0x400, {0,0,0,0,0,0,0,0}},	// Element 0: Driver Interface
											// 0		Next Screen Pressed
											// 1 		Cruise Enabled
											// 8-15 	Cruise control set speed

		{ 0x501, {0,0,0,0,0,0,0,0}},	// 1: Status Information
											// 0		Bridge PWM
											// 1		Motor Current
											// 2		Velocity
											// 3		Bus Current
											// 4		Bus Voltage Upper Limit
											// 5		Bus Voltage Lower Limit
											// 6		Headsink Temp
											// 7-15		Reserved
											// 16		Hardware Over Current
											// 17		Software Over Current
											// 18		DC Bus Over Voltage
											// 19		Bad Motor Position Hall Sequence
											// 20		Watchdog Caused Last Reset
											// 21		Config Read Error
											// 22		A 15V Rail Under Voltage Lock Out
											// 23-31	Reserved
											// 32-47	Active motor
											// 48-63	Reserved

		{ 0x503, {0,0,0,0,0,0,0,0}},	// 2: Velocity Measurement
											// 0-31		Motor Velocity
											// 32-63	Vehicle Velocity

		{ 0x50B, {0,0,0,0,0,0,0,0}},	// 3: Sink & Motor Temperature Measurement
											// 0-31		Motor temperature
											// 32-63	Surface temperature of controller heatsink


		{ 0x622, {0,0,0,0,0,0,0,0}},	// 4: Faults
											// 0		BMS Fault (0 = OK; 1 = BMS master is in the Fault State)
											// 1-4		Unused
											// 8-23		Time after power on
											// 24-28	Power from source
											// 29		HLIM is set: cannot charge
											// 30		LLIM is set: cannot discharge
											// 31		Fan is on
											// 32-39	Fault Code
											// 40		Driving off while plugged in
											// 41		Interlock is tripped
											// 42		Communication fault with cell
											// 43		Charge overcurrent
											// 44		Discharge overcurrent
											// 45		Over temperature
											// 46		Under voltage
											// 47		Over voltage
											// 48		Low voltage warning
											// 49		High voltage warning
											// 50		Charge overcurrent warning
											// 51		Discharge overcurrent warning
											// 52		Cold temperature warning
											// 53		Hot temperature warning
											// 54		Low SOH warning
											// 55		Isolation fault warning

		{ 0x623, {0,0,0,0,0,0,0,0}},	// 5: Voltage Status
											// 0-15		Total pack voltage
											// 16-23	Voltages of least charged cell
											// 24-31	ID of cell with lowest voltage
											// 32-39	Voltage of highest charged cell
											// 40-47	ID of cell with highest voltage

		{ 0x626, {0,0,0,0,0,0,0,0}}	,	// 6: Pack Health
											// 0-7		SOC
											// 8-23		Depth Of Discharge
											// 24-39	Capacity

		{ 0x627, {0,0,0,0,0,0,0,0}}		// 7: Temperature (Battery)
											// 0-7		Temperature
											// 8-15		Unusde
											// 16-23	Min Temperature
											// 24-31	ID of cell with lowest temperature
											// 32-39	Max Temperature
											// 40-47	ID of cell with highest temperature


};

/* FUNCTION DEFINITIONS ---------------------------------------------*/

/**
 * A Delay with a value of ~1ms
 * @Param counts: Number of milliseconds
 * Returns: nothing
 */
//TODO: Rewrite this function to use Timing Interrupts
void Delay(uint32_t counts) {
	uint32_t i;
	uint16_t j;
	for (i = 0; i < counts; i++) {
		for (j = 0; j < 16000; j++) {
		}
	}

}

/**
 * A Delay with a value of ~1us
 * @Param counts: Number of microseconds
 * Returns: nothing
 */
//TODO: Rewrite this function to use Timing Interrupts
void MicrosecondDelay(uint32_t counts) {
	uint32_t i;
	uint8_t j;

	for (i = 0; i < counts; i++) {
		for (j = 0; j < 16; j++) {
		}
	}
}

/**
 * Moves cursor to the position on the screen determined by XY Coordinates
 * @Param x: x-coordinate
 * @Param y: y-coordinate
 */
void MoveCursor(uint8_t x, uint8_t y) {
	/**
	 * The screen's dimension is 40 x 30, and the location of the
	 * cursor is described by the number in the CSRW register. This
	 * number is the "offeset" from the top left corner in row-major
	 * form
	 *
	 * Row major:
	 * 0 1 2 3 4
	 * 5 6 7 8 9
	 *
	 */
	uint16_t offset = 40 * y + x;

	TransmitCommand(0x46);
	// The CSRW register is 2 bytes long, so the first parameter
	// is the first byte and the second parameter is the second
	// byte
	TransmitCommandParameter(offset & 0xFF);
	TransmitCommandParameter(offset >> 8);
}

/**
 * Purpose: Prints 1200 " " characters to clear the screen
 */
void ClearScreen() {
	//Set cursor to start position
	MoveCursor(0, 0);

	//replace all characters with spaces
	TransmitCommand(0x42);
	for (uint16_t i = 0; i < 1200; i++) {
		TransmitCommandParameter(0x20);
	}
	MoveCursor(0, 0);
}

/**
 * Saves each subcharacter to the SG RAM 1, in locations 0x80 to 0x8F
 * @Param bitmap: An array of the bitmap of that particular subcharacter
 * @Param offset: Some offset number
 * Returns: nothing
 */
void SaveCharacterToRAM(uint8_t *bitmap, uint8_t offset) {
	TransmitCommand(0x46);
	TransmitCommandParameter(offset);
	TransmitCommandParameter(0x48);
	TransmitCommand(0x42);

	for (int i = 0; i < BYTEPERBITMAP; i++) {
		TransmitCommandParameter(bitmap[i]);
	}
}

/**
 * Writes custom bitmaps to the LCD screen RAM
 */
void CharacterBitMaps(void) {
	uint8_t bitmap[16][BYTEPERBITMAP] = { { 0x7E, 0x7E, 0x60, 0x60, 0x60, 0x60,
			0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x7E }, {
			0x7E, 0x7E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
			0x06, 0x06, 0x06, 0x7E, 0x7E }, { 0x7E, 0x7E, 0x60, 0x60, 0x60,
			0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60 },
			{ 0x7E, 0x7E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
					0x06, 0x06, 0x06, 0x06, 0x06 }, { 0x60, 0x60, 0x60, 0x60,
					0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
					0x7E, 0x7E }, { 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
					0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x7E, 0x7E }, {
					0x7E, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x7E, 0x7E },
			{ 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
					0x60, 0x60, 0x60, 0x60, 0x60 }, { 0x06, 0x06, 0x06, 0x06,
					0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
					0x06, 0x06 }, { 0x00, 0x06, 0x06, 0x0E, 0x0E, 0x1E, 0x36,
					0x36, 0x36, 0x66, 0x66, 0x66, 0xE6, 0xE6, 0x86, 0x00 }, {
					0x00, 0x62, 0x62, 0x62, 0x66, 0x66, 0x66, 0x6C, 0x6C, 0x6C,
					0x70, 0x70, 0x70, 0x60, 0x60, 0x00 }, { 0x00, 0x60, 0x60,
					0x70, 0x70, 0x78, 0x6C, 0x6C, 0x6C, 0x66, 0x66, 0x66, 0x67,
					0x67, 0x61, 0x00 },
			{ 0x00, 0x46, 0x46, 0x46, 0x66, 0x66, 0x66, 0x36, 0x36, 0x36, 0x0E,
					0x0E, 0x0E, 0x06, 0x06, 0x00 }, { 0xC0, 0xE0, 0x60, 0x30,
					0x30, 0x30, 0x18, 0x18, 0x0C, 0x0C, 0x04, 0x06, 0x06, 0x06,
					0x07, 0x03 }, { 0x03, 0x07, 0x06, 0x06, 0x0C, 0x0C, 0x18,
					0x18, 0x18, 0x30, 0x30, 0x60, 0x60, 0x60, 0xE0, 0xC0 } };
	TransmitCommand(0x5C);
	TransmitCommandParameter(0x00);
	TransmitCommandParameter(0x40);
	TransmitCommand(0x4C);

	for (uint8_t i = 0; i < 16; i++) {
		SaveCharacterToRAM(bitmap[i], i * BYTEPERBITMAP);
	}
}

/**
 * Outputs a string of characters to the screen
 * @Param Str[]: A string, "array of characters", whatever
 * @Param starting_x: x-coordinate to write the string to
 * @Param starting_y: y-coordinate to write the string to
 * Returns: nothing
 */
void OutputString(char Str[], uint8_t starting_x, uint8_t starting_y) {

	static uint8_t LOOKUPTABLE[LOOKUP_ROW][LOOKUP_COLUMN] = { //Lookup Table for all Characters

			{ 0x82, 0x83, 0x82, 0x83 }, { 0x80, 0x81, 0x84, 0x85 }, { 0x82,
					0x86, 0x84, 0x87 },
					{ 0x82, 0x83, 0x84, 0x85 }, //A B C D
					{ 0x80, 0x86, 0x84, 0x87 }, { 0x82, 0x86, 0x82, 0x86 }, {
							0x82, 0x86, 0x84, 0x81 },
					{ 0x84, 0x85, 0x88, 0x89 }, //E F G H
					{ 0x83, 0x86, 0x85, 0x87 }, { 0x20, 0x83, 0x87, 0x85 }, {
							0x89, 0x8F, 0x89, 0x8E },
					{ 0x88, 0x20, 0x84, 0x87 }, //I J K L
					{ 0x8C, 0x8A, 0x88, 0x89 }, { 0x8C, 0x89, 0x88, 0x8D }, {
							0x82, 0x83, 0x84, 0x85 },
					{ 0x80, 0x81, 0x88, 0x20 }, //M N O P
					{ 0x80, 0x81, 0x20, 0x89 }, { 0x80, 0x81, 0x88, 0x8E }, {
							0x80, 0x86, 0x87, 0x81 },
					{ 0x83, 0x86, 0x89, 0x20 }, //Q R S T
					{ 0x88, 0x89, 0x84, 0x85 }, { 0x88, 0x89, 0x8E, 0x8F }, {
							0x88, 0x89, 0x8B, 0x8D },
					{ 0x8E, 0x8F, 0x8F, 0x8E }, //U V W X
					{ 0x84, 0x85, 0x20, 0x85 },
					{ 0x86, 0x81, 0x80, 0x87 },                            //Y Z
					{ 0x82, 0x83, 0x84, 0x85 },
					{ 0x89, 0x20, 0x89, 0x20 },                            //0 1
					{ 0x86, 0x81, 0x80, 0x87 }, { 0x86, 0x81, 0x87, 0x85 }, {
							0x84, 0x85, 0x20, 0x89 },
					{ 0x80, 0x86, 0x87, 0x81 }, //2 3 4 5
					{ 0x82, 0x86, 0x80, 0x81 }, { 0x86, 0x83, 0x20, 0x89 }, {
							0x80, 0x81, 0x84, 0x85 },
					{ 0x80, 0x81, 0x20, 0x89 }, //6 7 8 9
					{ 0x20, 0x20, 0x20, 0x20 }, { 0x20, 0x20, 0x2E, 0x20 }, {
							0x2E, 0x20, 0x2E, 0x20 }, { 0x87, 0x87, 0x86, 0x86 }, //SPACE, ., :, -
			};

	uint8_t temp;
	uint8_t x = starting_x;

	TransmitCommand(0x4C);                 //Set the cursor direction to "Right"

	for (uint8_t c = 0; Str[c] != '\0'; c++)
	// used to be for (uint8_t c = 0; c < Str[c] != '\0'; c++)
			{
		MoveCursor(x, starting_y);
		temp = (uint8_t) Str[c];
		if (temp == 32) {
			temp = 36;                  //if character is " "
		} else if (temp == 58) {
			temp = 38;                  //if character is ":"
		} else if (temp == 46) {
			temp = 37;                  //if character is "."
		} else if (temp == 45) {
			temp = 39;                  //if character is "-"
		} else if (temp <= 57) {
			temp -= OFFSET_NUMERIC;     //if character is between "0" and "9"
		} else if (temp <= 90) {
			temp -= OFFSET_CAPITAL;     //if character is between "A" and "Z"
		} else if (temp <= 122) {
			temp -= OFFSET_LOWCASE;     //if character is between "a" and "z"
		}

		//TEMP IS NOW THE INDEX TO THE LOOKUP TABLE

		/***********************************
		 ** CHARACTER ASSEMBLY AND OUTPUT
		 *
		 * 1 character is
		 *  _ _
		 * |A B|
		 * |C D|
		 *  - -
		 * (4 subcharacters, in that order)     *
		 ***********************************/

		TransmitCommand(0x42);                      //print subcharacers A and B
		TransmitCommandParameter(LOOKUPTABLE[temp][0]);
		TransmitCommandParameter(LOOKUPTABLE[temp][1]);

		MoveCursor(x, starting_y + 1);

		TransmitCommand(0x42);                 //print the subcharacters C and D
		TransmitCommandParameter(LOOKUPTABLE[temp][2]);
		TransmitCommandParameter(LOOKUPTABLE[temp][3]);

		x += 3;
	}
}

/**
 * Converts a number to a string which can be output on the screen
 * The output can take any value from '-999.9' to ' 999.9'
 * @Param num: The integer portion of the number to be displayed
 * @Param dec: The decimal portion of the number to be displayed
 *              NOTE: the decimal will be off if the value if -1,
 *                    otherwise, the decimal will be on
 * @Param x: x-coordinate to write the character
 * @Param y: y-coordinate to write the character
 * Returns: nothing
 */
void OutputPaddedInteger(int32_t num, uint8_t dec, uint8_t x, uint8_t y) {
	uint8_t i;
	char str[5] = { ' ', ' ', ' ', ' ', '\0' };
	char decplace[2] = { ' ', '\0' };

	if (num < 0) {
		str[0] = '-';
		num = -1 * num;
	} else if (num == 0) {
		str[3] = '0';
	}

	if (num > 999) {
		num = 999;
	}

	// Convert the integer digits into chars, one by one
	for (i = 3; num != 0; i--) {
		str[i] = "0123456789"[num % 10];
		num = num / 10;
	}

	OutputString(str, x, y);

	if (dec != -1) {
		//Output 1 decimal place
		OutputString(".", x + 12, y);

		dec = dec % 10;

		decplace[0] = "0123456789"[dec];
		OutputString(decplace, x + 15, y);
	}
}

/**
 * Periodically called to change the value of the Charge or Speed display bar
 * @Param num: The number to be represented
 * @Param max: The maximum value that the bar can display
 * @Param y: The y-coordinate of the bar
 * Returns: nothing
 */
void SetBar(uint8_t num, uint8_t max, uint8_t y) {

	uint8_t blocks = 4 * (num * 10 / max) + (num % 10 >= 5) * 2;

	MoveCursor(0, y);
	TransmitCommand(0x4C);
	TransmitCommand(0x42);

	for (uint8_t i = 0; i < blocks; i++) {
		TransmitCommandParameter(0x1A);
	}

	for (uint8_t i = blocks; i < 40; i++) {
		TransmitCommandParameter(0x20);
	}
}

/**
 * Displays all information in a single screen
 */
void DisplayScreen(void) {
	ClearScreen();

		// **TEMPORARY: DATA IS DISPLAYED 1 BY 1


	// Different code for each page allows for different designs
	switch (current_page) {
		case (1):

			for (uint8_t i = 0; i < NUM_ROWS_PAGE1; i++) {

				// Convert output value from 8 uint8_t bytes of raw data to a single int32_t value

				union FloatBytes convert;	// Used to convert the data arrays sent by the MDU and BMS
				int32_t output = 0;			// Used to store the output data which will be displayed after the switch statement

				switch(i)
				{
					// BATT VOLT
					case 0:
						// First 2 bytes in kV (big endian) converted into a 32 bit voltage in kV
						convert.bytes[0] = DisplayDataArray[5].raw_data[0];
						convert.bytes[1] = DisplayDataArray[5].raw_data[1];
						output = convert.float_value;
						break;

					// BATT TEMP
					case 1:
						// Temperature is the first byte as the average pack temperature (signed -127C to 127C)
						// Sign +/- is denoted by 2's complement (below is a conversion to decimal)

						// Check sign bit for negative value: first bit will be a 1
						if (DisplayDataArray[6].raw_data[0] & 0B10000000)
						{
							// Invert byte, remove sign bit, and subtract 127
							output = ((~DisplayDataArray[6].raw_data[0]) & 0B01111111) - 127;
						}
						else
						{
							output = DisplayDataArray[6].raw_data[0];
						}
						break;

					// SPEED
					case 2:
						// Last 4/8 bytes represent the velocity in m/s
						convert.bytes[0] = DisplayDataArray[2].raw_data[4];
						convert.bytes[1] = DisplayDataArray[2].raw_data[5];
						convert.bytes[2] = DisplayDataArray[2].raw_data[6];
						convert.bytes[3] = DisplayDataArray[2].raw_data[7];

						// Convert m/s to km/h
						output = convert.float_value * ((int8_t)18/5);
						break;

					// SOC
					case 3:
						// Charge is the first byte as a percentage value
						output = DisplayDataArray[6].raw_data[0];
						break;
				}

				// Display static and dynamic data (note: the only changing value is the output variable set above)
				OutputString(DisplayLayoutPage1[i].name, 0,	DisplayLayoutPage1[i].ypos);
				OutputPaddedInteger(output, -1, DisplayLayoutPage1[i].data_xpos, DisplayLayoutPage1[i].ypos);
				OutputString(DisplayLayoutPage1[i].unit, DisplayLayoutPage1[i].unit_xpos, DisplayLayoutPage1[i].ypos);
			}
			break;

		case (2):

			for (uint8_t i = 0; i < NUM_ROWS_PAGE2; i++) {

				// Convert output value from 8 uint8_t bytes of raw data to a single int32_t value

				union FloatBytes convert;	// Used to convert the data arrays sent by the MDU and BMS
				int32_t output = 0;			// Used to store the output data which will be displayed after the switch statement

				switch(i)
				{
					// MOTOR TEMP
					case 0:
						// First 4 bytes represent the motor temperature in C
						convert.bytes[0] = DisplayDataArray[2].raw_data[0];
						convert.bytes[1] = DisplayDataArray[2].raw_data[1];
						convert.bytes[2] = DisplayDataArray[2].raw_data[2];
						convert.bytes[3] = DisplayDataArray[2].raw_data[3];
						output = convert.float_value;
						break;

					// MOTOR VEL
					case 1:
						// First 4 bytes represent the velocity in RPM
						convert.bytes[0] = DisplayDataArray[2].raw_data[0];
						convert.bytes[1] = DisplayDataArray[2].raw_data[1];
						convert.bytes[2] = DisplayDataArray[2].raw_data[2];
						convert.bytes[3] = DisplayDataArray[2].raw_data[3];
						output = convert.float_value;
						break;

					// BMS FAULT
					case 2:
						// Check the first bit
						if (DisplayDataArray[4].raw_data[0] & 0B00000001)
						{
							output = 1;
						}
						else
						{
							output = 0;
						}
						break;

					// CRUISE ON
					case 3:
						// Check the second bit
						if (DisplayDataArray[0].raw_data[0] & 0B0000010)
						{
							output = 1;
						}
						else
						{
							output = 0;
						}
						break;
				}

				// Display static and dynamic data (note: the only changing value is the output variable set above)
				OutputString(DisplayLayoutPage2[i].name, 0,	DisplayLayoutPage2[i].ypos);
				OutputPaddedInteger(output, -1, DisplayLayoutPage2[i].data_xpos, DisplayLayoutPage2[i].ypos);
				OutputString(DisplayLayoutPage2[i].unit, DisplayLayoutPage2[i].unit_xpos, DisplayLayoutPage2[i].ypos);
			}
			break;

	}

}

/**
 * Erases and revalues a single value field on a screen
 * @Param integerValue: The integer value of the parameter(Between -999 to 999)
 * @Param decValue: The decimal component of the parameter
 * @Param x: The x value of the parameter on the screen
 * @Param y: The y value of the parameter on the screen
 */
void UpdateScreenParameter(int32_t integerValue, uint8_t decValue, uint8_t x,
		uint8_t y) {
	//Clear a 6 x 2 rectangle to erase the previous number
	OutputString("     ", x, y);

	//Insert the new number
	OutputPaddedInteger(integerValue, decValue, x, y);
}

/**
 * Write a byte of data through C3 to C10(Data Buses)
 * @Param byte: 1 byte code
 * Returns: nothing
 */
void WriteByteToDataBus(uint8_t byte) {
	GPIOC->BSRR = byte << 3;
	GPIOC->BRR = ((uint8_t) (~byte)) << 3;
}

/**
 * Writes a byte of command code through ports C3 to C10(Data Buses)
 * @Param command_code: 1 byte command code
 * returns: nothing
 */
void TransmitCommand(uint8_t command_code) {
	GPIOC->BSRR = 0x1UL << 0;                   //SET C0 to HIGH
	GPIOC->BRR = 0x1UL << 1;                    //SET C1 to LOW
	WriteByteToDataBus(command_code);
	MicrosecondDelay(1);
	GPIOC->BSRR = 0x1UL << 1;                   //SET C1 to HIGH
	MicrosecondDelay(1);
}

/**
 * Writes a byte of parameter code through ports C3 to C10(Data Buses)
 * @Param parameter_code: 1 byte parameter code
 * returns: nothing
 */
void TransmitCommandParameter(uint8_t parameter_code) {
	GPIOC->BRR = 0x1UL << 0;                    //SET C0 to LOW
	GPIOC->BRR = 0x1UL << 1;                    //SET C1 to LOW
	WriteByteToDataBus(parameter_code);
	MicrosecondDelay(1);
	GPIOC->BSRR = 0x1UL << 1;                   //SET C1 to HIGH
	MicrosecondDelay(1);
}

/**
 * Initialise GPIO Pins C0 to C12 for output
 */
void InitialiseLCDPins(void) {

	//Setup System Clock C
	RCC->APB2ENR &= 0;
	RCC->APB2ENR |= 0x1UL << 4;

	//Setup Pins C5 - C12 as OUTPUT
	GPIOC->CRL &= 0;
	GPIOC->CRH &= 0;
	GPIOC->CRL |= 0x33333333UL; //Initialise C0 to C7
	GPIOC->CRH |= 0x33333UL; //Initialise C8 to C12

	//Set Pin initial values
	GPIOC->BSRR = 0x1UL << 0;    // C0 HIGH
	GPIOC->BSRR = 0x1UL << 1;    // C1 HIGH
	GPIOC->BSRR = 0x1UL << 2;    // C2 HIGH
	GPIOC->BRR = 0x1UL << 11;    // C11 LOW
	GPIOC->BSRR = 0x1UL << 12;   //C12 HIGH

}

/**
 * Initialise screen settings and character bit maps
 */
void ScreenSetup(void) {
	// system setup (section 6-2-1 on raio datasheet)
	TransmitCommand(0x40);
	Delay(5);
	// 0 0 IV 1 W/S M2 M1 M0
	TransmitCommandParameter(0x34);
	// WF 0 0 0 0 FX FX FX
	TransmitCommandParameter(0x87);
	// 0 0 0 0 FY FY FY FY
	TransmitCommandParameter(0xF);
	// C/R (horizontal bytes per line)
	TransmitCommandParameter(39);
	// TC/R (horizontal bytes per line, incl blanking)
	TransmitCommandParameter(47);
	// L/F (lines per frame)
	TransmitCommandParameter(239);
	// APL (horizontal address range, LSB)
	TransmitCommandParameter(40);
	// APH (horizontal address range, MSB)
	TransmitCommandParameter(0);

	// scroll parameters
	TransmitCommand(0x44);
	TransmitCommandParameter(0); // SAD 1L
	TransmitCommandParameter(0); // SAD 1H
	TransmitCommandParameter(240); // SL1
	TransmitCommandParameter(176); // SAD 2L
	TransmitCommandParameter(4); // SAD 2H
	TransmitCommandParameter(240); // SL2
	TransmitCommandParameter(0); // SAD 3L
	TransmitCommandParameter(0); // SAD 3H
	TransmitCommandParameter(0); // SAD 4
	TransmitCommandParameter(0); // SAD 4

	TransmitCommand(0x5B);
	TransmitCommandParameter(0);

	// set horizontal scroll position
	TransmitCommand(0x5A);
	TransmitCommandParameter(0);

	// set display overlay format
	TransmitCommand(0x5B);
	TransmitCommandParameter(0);

	// turn off the display and configure cursor
	TransmitCommand(0x58);
	TransmitCommandParameter(0x57);

	// set cursor size & type
	TransmitCommand(0x5D);
	TransmitCommandParameter(4);
	TransmitCommandParameter(0x84);

	// set cursor direction
	TransmitCommand(0x4C);

	// turn on the display and configure cursor
	TransmitCommand(0x59);
	TransmitCommandParameter(0x7);

	CharacterBitMaps();

}
