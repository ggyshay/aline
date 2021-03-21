#include "DisplayDriver.h"

char DisplayDriver::dataBuffer[1024] = {};
bool DisplayDriver::pendingTransmission = false;

void DisplayDriver::sendCmd(unsigned char cmd)
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

    sendCmd(0x20); // modo de endereçamento
    sendCmd(0x01); // horizontal, 02 é page

    sendCmd(0x21); // range da coluna
    sendCmd(0x00); // inicial é 0
    sendCmd(0x7f); // final é 7f

    sendCmd(0x22); // range da pagina
    sendCmd(0x00); // inicial é 0
    sendCmd(0x01); // final é 7

    sendCmd(0x40); // start line 0

    sendCmd(0xc8); // com mode normal
    sendCmd(0xa1); // remap normal

    sendCmd(0x81); // contraste, acho que não precisa dele
    sendCmd(0xFF);

    sendCmd(0xAF);

    sendCmd(0x8D); //--set vcomh
    sendCmd(0x14); //0x20,0.77xVcc
    delay(50);
    Wire.onTransmitDone(triggerTransmission);

    Wire.beginTransmission(0x3c);
    Wire.write(0x00);
    Wire.write(0x20);
    Wire.write(0x00);
    Wire.write(0x21);
    Wire.write(0x00);
    Wire.write(0x7f);
    Wire.write(0x22);
    Wire.write(0);
    Wire.write(0x07);
    Wire.sendTransmission();
    Wire.finish();
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