#include "DisplayDriver.h"
void print_i2c_status(void)
{
    switch (Wire1.status())
    {
    case I2C_WAITING:
        // Serial.print("I2C waiting, no errors\n");
        break;
    case I2C_ADDR_NAK:
        Serial.print("Slave addr not acknowledged\n");
        break;
    case I2C_DATA_NAK:
        Serial.print("Slave data not acknowledged\n");
        break;
    case I2C_ARB_LOST:
        Serial.print("Bus Error: Arbitration Lost\n");
        break;
    case I2C_TIMEOUT:
        Serial.print("I2C timeout\n");
        break;
    case I2C_BUF_OVF:
        Serial.print("I2C buffer overflow\n");
        break;
    case I2C_NOT_ACQ:
        Serial.print("I2C: Not acq\n");
        break;
    case I2C_DMA_ERR:
        Serial.print("DMA error\b");
        break;
    case I2C_SENDING:
        Serial.println("i2c sending");
        break;
    case I2C_SEND_ADDR:
        Serial.println("i2c send addr");
        break;
    case I2C_RECEIVING:
        Serial.println("i2c recieving");
        break;
    default:
        Serial.print("I2C busy\n");
        break;
    }
}

char DisplayDriver::dataBuffer[1024] = {};
bool DisplayDriver::pendingTransmission = false;

void DisplayDriver::sendCmd(byte cmd)
{
    Wire.beginTransmission(0x3c);
    Wire.write(0x00);
    Wire.write(cmd);
    Wire.sendTransmission();
    Wire.finish();
}

void DisplayDriver::init()
{
    Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, 400000);
    Wire.setOpMode(I2C_OP_MODE_DMA);
    delay(100);

    sendCmd(0xAE); // desliga display

    sendCmd(0x20); // modo de endereçamento
    sendCmd(0x00); // horizontal, 02 é page

    sendCmd(0x21); // range da coluna
    sendCmd(0x00); // inicial é 0
    sendCmd(0x7f); // final é 7f

    sendCmd(0x22); // range da pagina
    sendCmd(0x00); // inicial é 0
    sendCmd(0x07); // final é 7

    sendCmd(0xc8); // com mode normal
    sendCmd(0xa1); // remap normal

    sendCmd(0x81); // contraste, acho que não precisa dele
    sendCmd(0xFF);

    sendCmd(0x40); // start line 0

    sendCmd(0xAF); // liga display

    sendCmd(0x8D); //--set vcomh
    sendCmd(0x14); // 0x20,0.77xVcc
    delay(50);
    Wire.onTransmitDone(triggerTransmission);
}

unsigned int DisplayDriver::lastTransmit = 0;

void DisplayDriver::triggerTransmission()
{
    if (pendingTransmission && Wire.done())
    {
        transmitScreen();
        pendingTransmission = false;
    }
    else if (!Wire.done() && micros() - lastTransmit > 125000)
    {
        print_i2c_status();
        Serial.printf("stuck for %d micros\n", micros() - lastTransmit);
        Wire.resetBus();
    }
}

void DisplayDriver::transmitScreen()
{
    Wire.beginTransmission(0x3c);
    Wire.write(0x40);
    transmitBuffer();
    lastTransmit = micros();
    Wire.sendTransmission();
}

void DisplayDriver::transmitBuffer()
{
    for (int i = 0; i < 1024; ++i)
    {
        Wire.write(dataBuffer[i]);
    }
}

void DisplayDriver::putScreen()
{
    pendingTransmission = true;
    triggerTransmission();
}