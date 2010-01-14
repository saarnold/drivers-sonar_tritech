#include "SonarInterface.h"
#include <stdio.h>
#include <string.h>

#define WRITETIMEOUT 200

SonarInterface::SonarInterface(): 
  IODriver(MAX_PACKET_SIZE,false)
{
    //Device Adresses
    rxNode = 0x02;
    txNode = 0xFF;
    timeCnt=0;
    fileCnt=0;
    headDataChanged=false;
}

SonarInterface::~SonarInterface() {
}

void SonarInterface::requestVersion()
{
    sendPacked(mtSendVersion,0);
}

void SonarInterface::requestData() {
    if(headDataChanged)
   	sendPacked(mtHeadCommand,headData);
    sendPacked(mtSendData,0);
}

void SonarInterface::sendHeadData(bool adc8on,bool cont,bool scanright,bool invert,bool chan2,bool applyoffset,
		bool pingpong,uint16_t rangeScale, uint16_t leftLimit, uint16_t rightLimit, uint8_t adSpan, 
		uint8_t adLow, uint8_t initialGain, uint8_t motorStepDelayTime, uint8_t motorStepAngleSize,
		uint16_t adInterval, uint16_t numberOfBins, uint16_t adcSetpoint) {

    
    uint8_t V3BParams = 0x1D;
    //bool adc8on = true;
    //bool cont = true;
    //bool scanright = false;
    //bool invert = true;
    bool motorOff = false;
    bool txOff= false;
    bool spare = false;
    //bool chan2 = true;
    bool raw = true;
    bool hasmot = true;
    //bool applyoffset = false;
    //bool pingpong = false;
    bool starellim = false;
    bool replyasl = true;
    bool replyThr = false;
    bool ignoreSensor = false;
    uint8_t headType = 0x02;
    uint32_t txnCh1 = 0; //Ignored
    uint32_t txnCh2 = 0; //Ignored
    uint32_t rxnCh1 = 0; //Ignored
    uint32_t rxnCh2 = 0; //Ignored
    uint16_t pulseLength = (0x28) | 0; //Ignored
    //uint16_t rangeScale  = (0x3C) | 0; //6 Meter
    //uint16_t leftLimit   = (0x01) | 0; //1
    //uint16_t rightLimit  = (0xFF) | (0x18<<8);
    //uint8_t adSpan = 0x51;
    //uint8_t adLow = 0x8;
    //uint8_t initialGainCh1 = 0x90;//54;
    //uint8_t initialGainCh2 = 0x90;//54;
    uint8_t initialGainCh1 = initialGain;
    uint8_t initialGainCh2 = initialGain;
    uint16_t slopeCh1 = 0; //Ignored
    uint16_t slopeCh2 = 0; //Ignored
    //uint8_t motorStepDelayTime = 0x19;
    //uint8_t motorStepAngleSize = 0x10;
    //uint16_t adInterval = (0x8D) | (0<<8);
    //uint16_t numberOfBins = (0xFF) | (0<<8);
    uint16_t maxADBuff = (0xE8) | (3<<8);
    uint16_t lockoutTime = (0x97) | (0x03<<8);
    uint16_t minorAxisDir = (0x40) | (0x06<<8);
    uint8_t majorAxisPan = (0x01);
    uint8_t crtl2 = 0;
    uint16_t scanZ = 0;
    
    //uint8_t adSpanCh1 = 0x50;
    //uint8_t adSpanCh2 = 0x51;
    //uint8_t adLowCh1 = 0x09;
    //uint8_t adLowCh2 = 0x08;
    //uint8_t initialGainCh1_2 = 0x90;//0x54;
    //uint8_t initialGainCh2_2 = 0x90;//0x54;
    //uint8_t adcSetpointCh1 = 0x00;
    //uint8_t adcSetpointCh2 = 0x00;
    
    uint8_t adSpanCh1 = adSpan;
    uint8_t adSpanCh2 = adSpan;
    uint8_t adLowCh1 = adLow;
    uint8_t adLowCh2 = adLow;
    uint8_t initialGainCh1_2 = initialGain;
    uint8_t initialGainCh2_2 = initialGain;
    uint8_t adcSetpointCh1 = adcSetpoint;
    uint8_t adcSetpointCh2 = adcSetpoint;

    uint16_t slopeCh1_2 =0; //Ignored
    uint16_t slopeCh2_2 =0; //Ignored
    uint16_t slopeDelayCh1 =0; //Ignored
    uint16_t slopeDelayCh2 =0; //Ignored

    uint16_t headControl =
        (adc8on?ADC8ON:0) |
        (cont?CONT:0) |
        (scanright?SCANRIGHT:0) |
        (invert?INVERT:0) |
        (motorOff?MOTOFF:0) |
        (txOff?TXOFF:0) |
        (spare?SPARE:0) |
        (chan2?CHAN2:0) |
        (raw?RAW:0) |
        (hasmot?HASMOT:0) |
        (applyoffset?APPLYOFFSET:0) |
        (pingpong?PINGPONG:0) |
        (starellim?STARELLIM:0) |
        (replyasl?REPLYASL:0) |
        (replyThr?REPLYTHR:0) |
        (ignoreSensor?IGNORESENSOR:0)
        ;

    //headControl = (headControl&255)<<8 | (headControl>>8)&255;

    int pos=0;
    memcpy(headData,&V3BParams,1);
    pos+=1;
    memcpy((headData+pos),&headControl,2);
    pos+=2;
    memcpy((headData+pos),&headType,1);
    pos+=1;
    memcpy((headData+pos),&txnCh1,4);
    pos+=4;
    memcpy((headData+pos),&txnCh2,4);
    pos+=4;
    memcpy((headData+pos),&rxnCh1,4);
    pos+=4;
    memcpy((headData+pos),&rxnCh2,4);
    pos+=4;
    memcpy((headData+pos),&pulseLength,2);
    pos+=2;
    memcpy((headData+pos),&rangeScale,2);
    pos+=2;
    memcpy((headData+pos),&leftLimit,2);
    pos+=2;
    memcpy((headData+pos),&rightLimit,2);
    pos+=2;
    memcpy((headData+pos),&adSpan,1);
    pos++;
    memcpy((headData+pos),&adLow,1);
    pos++;
    memcpy((headData+pos),&initialGainCh1,1);
    pos++;
    memcpy((headData+pos),&initialGainCh2,1);
    pos++;
    memcpy((headData+pos),&slopeCh1,2);
    pos+=2;
    memcpy((headData+pos),&slopeCh2,2);
    pos+=2;
    memcpy((headData+pos),&motorStepDelayTime,1);
    pos++;
    memcpy((headData+pos),&motorStepAngleSize,1);
    pos++;
    memcpy((headData+pos),&adInterval,2);
    pos+=2;
    memcpy((headData+pos),&numberOfBins,2);
    pos+=2;
    memcpy((headData+pos),&maxADBuff,2);
    pos+=2;
    memcpy((headData+pos),&lockoutTime,2);
    pos+=2;
    memcpy((headData+pos),&minorAxisDir,2);
    pos+=2;
    memcpy((headData+pos),&majorAxisPan,1);
    pos++;
    memcpy((headData+pos),&crtl2,1);
    pos++;
    memcpy((headData+pos),&scanZ,2);
    pos+=2;
    memcpy((headData+pos),&adSpanCh1,1);
    pos++;
    memcpy((headData+pos),&adSpanCh2,1);
    pos++;
    memcpy((headData+pos),&adLowCh1,1);
    pos++;
    memcpy((headData+pos),&adLowCh2,1);
    pos++;
    memcpy((headData+pos),&initialGainCh1_2,1);
    pos++;
    memcpy((headData+pos),&initialGainCh2_2,1);
    pos++;
    memcpy((headData+pos),&adcSetpointCh1,1);
    pos++;
    memcpy((headData+pos),&adcSetpointCh2,1);
    pos++;
    memcpy((headData+pos),&slopeCh1_2,2);
    pos+=2;
    memcpy((headData+pos),&slopeCh2_2,2);
    pos+=2;
    memcpy((headData+pos),&slopeDelayCh1,2);
    pos+=2;
    memcpy((headData+pos),&slopeDelayCh2,2);
    pos+=2;

	//printf("Request change to %i bins\n",numberOfBins);
/*
fprintf(stdout,"\n%s,%s,%s,%s,%s,%s,%s,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n",mcr->getadc8on()?"true":"false",mcr->getcont()?"true":"false",
  mcr->getscanright()?"true":"false",mcr->getinvert()?"true":"false",mcr->getchan2()?"true":"false",mcr->getapplyoffset()?"true":"false",
  mcr->getpingpong()?"true":"false",mcr->getrangeScale(),mcr->getleftLimit(),mcr->getrightLimit(),mcr->getadSpan(),mcr->getadLow(),mcr->getinitialGain(),
  mcr->getmotorStepDelayTime(),mcr->getmotorStepAngleSize(),mcr->getadInterval(),mcr->getnumberOfBins(),mcr->getadcSetpointCh());
  */
  /*
  fprintf(stdout,"Head command data:\n");
  for(int i=0;i<68;i++){
    	fprintf(stdout,"%02X ",data[i]);
   }
   fprintf(stdout,"Head command data Finished.\n");
  */
   headDataChanged=true;
   sendPacked(mtHeadCommand,headData);
}

void SonarInterface::sendPacked(MsgType type, uint8_t *data) {

    uint16_t length = 5+MsgLength[type]; //TODO
    uint8_t msg[length+6];
    //Header
    msg[0] = '@';
    //Length in ASCII and binary
    snprintf((char*)msg+1,5,"%04X",length);

    //printf((char*)msg+1);

    msg[5] = length & 255;
    msg[6] = (length>>8) & 255;
    msg[7] = txNode;
    msg[8] = rxNode;
    msg[9] = MsgLength[type];
    switch (type) {

    case mtNull:
        fprintf(stderr,"Cannot create null-Packet\n");
        break;
        /*
        case mtHeadData:
        	break;
        case mtSpectData:
        	break;
        case mtAlive:
        	break;
        case mtPrgAck:
        	break;
        case mtBBUserData:
        	break;
        case mtTestData:
        	break;
        case mtAuxData:
        	break;
        case mtAdcData:
        	break;
        case mtLdcReq:
        	break;
        case mtLanStatus:
        	break;
        case mtSetTime:
        	break;
        case mtTimeout:
        	break;
        case mtReBoot:
        	break;
        case mtPerformanceData:
        	break;
        	*/
    case mtHeadCommand:
        msg[10] = mtHeadCommand;
        msg[11] = 0x80;
        msg[12] = rxNode;
        memcpy(msg+13,data,68);
        msg[81] = 0x0A;
        break;
        /*
        case mtEraseSector:
        	break;
        case mtProgBlock:
        	break;
        case mtCopyBootBlk:
        	break;*/
    case mtSendVersion:
    {
        msg[10] = mtSendVersion;
        msg[11] = 0x80;
        msg[12] = rxNode;
        msg[13] = 0x0A;
        break;
    }
    /*
    case mtSendBBUser:
    	break;
    */
    case mtSendData:
    {
        msg[10] = mtSendData;
        msg[11] = 0x80;
        msg[12] = rxNode;
        uint32_t time = 134145+timeCnt++; //TODO zeit ausm system holen
        memcpy(msg+13,&time,4);
        msg[17]=0x0A;
        break;
    }
    /*case mtSendPreferenceData:
    	break;*/
    default:
        fprintf(stderr,"Cannot create unknown packet with id: %02X \n",type);
    }

    
    bool written = writePacket(msg,length+6,WRITETIMEOUT);
	

    if (!written) {
        fprintf(stderr,"Couldn't send Packet\n");
    } else {
        /*
	fprintf(stdout,"Sended packet with length: %i\n",(length+6));
        for (int i=0;i< length+6;i++) {
            fprintf(stdout,"%02X ",msg[i]);
        }
        fprintf(stdout,"\n");
	*/
    }

}

void SonarInterface::processSerialData(){
	uint8_t packed[MAX_PACKET_SIZE];
	try{
		readPacket(packed,MAX_PACKET_SIZE,2000,2000);
      	}catch(timeout_error t) {
		printf("Timeout\n");
	}
	processMessage(packed);
}

/**
 * Given message started on binary lengh field
 **/
void SonarInterface::processMessage(uint8_t *message) {

    uint8_t type = message[10];

    switch (type) {

    case mtNull:
        fprintf(stderr,"Cannot handle null-Packet\n");
        break;
    case mtVersionData:
    {
        softwareVersion = message[13];
        infoBits = message[14];
        serialNr = message[15] | (message[16] <<8);
        programmLength = message[17] | (message[18] <<8) | (message[19] <<16) | (message[20] <<24);
        programmChecksum = message[21] | (message[22] << 8);
        nodeID = message[23];
        fprintf(stdout,"Software Version: 0x%02X\n",softwareVersion);
        fprintf(stdout,"InfoBits: 0x%02X\n",infoBits);
        fprintf(stdout,"Serial Number: %u\n",serialNr);
        fprintf(stdout,"Programm Length: %u\n",programmLength);
        fprintf(stdout,"Programm Checksum: %u\n",programmChecksum);
        fprintf(stdout,"Node ID: %u\n",nodeID);
        break;
    }
    case mtHeadData:
    {
    	
    	uint16_t packedSize = message[13] | (message[14]<<8);
	uint8_t deviceType = message[15];
	uint8_t headStatus = message[16];
	uint8_t sweepCode = message[17];
	uint16_t headControl = message[18] | (message[19]<<8);
	uint16_t range = message[20] | (message[21]<<8);
	uint32_t txn = message[22] | (message[23]<<8) | (message[24]<<16) | (message[25]<<24);
	uint8_t gain = message[26];
	uint16_t slope = message[27] | (message[28]<<8);
	uint8_t adSpawn = message[29];
	uint8_t adLow = message[30];
	uint16_t headingOffset = message[31] | (message[32]<<8);
	uint16_t adInterval = message[33] | (message[34]<<8);
	uint16_t leftLimit = message[35] | (message[36]<<8);
	uint16_t rightLimit = message[37] | (message[38]<<8);
	uint8_t steps = message[39];
	uint16_t bearing = message[40] | (message[41]<<8);
    	uint16_t dataBytes = message[42] | (message[43]<<8);
	uint8_t *scanData = message+44;
	
	//uint8_t debug[dataBytes];
	
	//for(int i=0;i<dataBytes;i++){
	//	debug[i]=i;
	//}
	
	SonarScan *scan = new SonarScan(packedSize,deviceType,headStatus,sweepCode,headControl,range,txn,gain,slope,adSpawn,adLow,headingOffset,adInterval,leftLimit,rightLimit,steps,bearing,dataBytes,scanData);

	notifyPeers(scan);
	
        uint8_t data[dataBytes];
        memcpy(data,scanData,dataBytes);
        //fprintf(stdout,"Actual Bearing: %u.\n",bearing);
        //fprintf(stdout,"DataBytes recived: %u.\n",dataBytes);
      	 
	requestData();

        //fprintf(stderr,"Cannot handle HeadData-Packet\n");
        break;
    }
    case mtSpectData:
        fprintf(stderr,"Cannot handle SpectData-Packet\n");
        break;
    case mtAlive:
        requestData();
        fprintf(stderr,"Got an Alive packet, Found Sonar!\n");
        break;
    case mtPrgAck:
        fprintf(stderr,"Cannot handle PrgAck-Packet\n");
        break;
    case mtBBUserData:
        fprintf(stderr,"Cannot handle BBUserData-Packet\n");
        break;
    case mtTestData:
        fprintf(stderr,"Cannot handle TestrData-Packet\n");
        break;
    case mtAuxData:
		{
    	uint16_t packedSize = message[13] | (message[14]<<8);
		float depth=0;
		if(packedSize == 9 && message[18] == '.' && message[22] == 'm'){
			depth+= ((message[15]-48)) * 100;
			depth+= ((message[16]-48)) * 10;
			depth+= ((message[17]-48));
			depth+= ((message[19]-48))/10.0;
			depth+= ((message[20]-48))/100.0;
			depth+= ((message[21]-48))/1000.0;
		}else{
			fprintf(stderr,"Cannot HAndle Unknown-AUX port data %s:%s",__FUNCTION__,__FILE__);
		}
		
		//for(int i=10;i<packedSize+10;i++){
		//	fprintf(stdout,"%c ",message[i]);
		//}
    		notifyPeers(depth);    
		break;
		}
    case mtAdcData:
        fprintf(stderr,"Cannot handle AdcData-Packet\n");
        break;
    case mtLdcReq:
        fprintf(stderr,"Cannot handle LdcReq-Packet\n");
        break;
    case mtLanStatus:
        fprintf(stderr,"Cannot handle LanStatus-Packet\n");
        break;
    case mtSetTime:
        fprintf(stderr,"Cannot handle SetTime-Packet\n");
        break;
    case mtTimeout:
        fprintf(stderr,"Cannot handle Timeout-Packet\n");
        break;
    case mtReBoot:
        fprintf(stderr,"Cannot handle ReBoot-Packet\n");
        break;
    case mtPerformanceData:
        fprintf(stderr,"Cannot handle PerformanceData-Packet\n");
        break;
    case mtHeadCommand:
        fprintf(stderr,"Cannot handle HeadCommand-Packet\n");
        break;
    case mtEraseSector:
        fprintf(stderr,"Cannot handle EraseSector-Packet\n");
        break;
    case mtProgBlock:
        fprintf(stderr,"Cannot handle ProgBlock-Packet\n");
        break;
    case mtCopyBootBlk:
        fprintf(stderr,"Cannot handle CopyBootBlock-Packet\n");
        break;
    case mtSendVersion:
        fprintf(stderr,"Cannot handle SendVersion-Packet\n");
        break;
    case mtSendBBUser:
        fprintf(stderr,"Cannot handle SendBBUser-Packet\n");
        break;
    case mtSendData:
        fprintf(stderr,"Cannot handle SendData-Packet\n");
        break;
    case mtSendPreferenceData:
        fprintf(stderr,"Cannot handle SendPreferenceData-Packet\n");
        break;
    default:
        fprintf(stderr,"Cannot Handle unknown packet\n");
    }
}


void SonarInterface::registerHandler(SonarHandler *handler){
	handlers.push_back(handler);
}

void SonarInterface::unregisterHandler(SonarHandler *handler){
	handlers.remove(handler);
}


void SonarInterface::notifyPeers(float newDepth){
	for(std::list<SonarHandler*>::iterator it =  handlers.begin(); it != handlers.end();it++){
		(*it)->processDepth(newDepth);
	}
}

void SonarInterface::notifyPeers(SonarScan *scan){
	for(std::list<SonarHandler*>::iterator it = handlers.begin(); it != handlers.end();it++){
		(*it)->processSonarScan(scan);
	}
}

bool SonarInterface::init(std::string const &port){
	return openSerial(port,115200);
}

int SonarInterface::getReadFD() {
	return getFileDescriptor();
}

int SonarInterface::extractPacket(uint8_t const* buffer, size_t buffer_size) const{

//	if(buffer_size > 2000) { printf("HUCH: %i,",buffer_size); return -1;}
//	printf("size: %i \n",buffer_size);
	
	if(buffer_size<7) return 0;
	
	int readPos=0;
	while (buffer[readPos] != '@' && readPos < buffer_size) {
		readPos++;
        }
	
/*	if(readPos > buffer_size){
		fprintf(stdout,"Cannot detect Package start readPos: %i, bufferSize %i %02X\n",readPos, buffer_size, buffer[0]);
		return 0;
	}
*/
	if(readPos>0){
		//fprintf(stdout,"Skipping %i bytes.\n",readPos);
            	//for (int i=0;i<readPos;i++)
                //	fprintf(stderr,"%02X ",buffer[i]);
		//fprintf(stderr,"\n");
		return -readPos;
	}
	//Double check length... with other lengh information
        uint16_t len = (buffer[5] | (buffer[6]<<8 )) +5;
	uint16_t hexlen = 0;
	char hexsum[5];
	memcpy(hexsum,buffer+1,4);
	hexsum[4]=0;

	sscanf(hexsum,"%x",(unsigned int*)&hexlen);
//	printf("bla: %c,%c,%c,%c\n",buffer[1],buffer[2],buffer[3],buffer[4]);
//	printf("Checks: %i,%i\n",len, hexlen);
	if(len-5 != hexlen){
		//printf("Kaput\n");
		//Seems this is not the real packed start so we skip...
		return -1;
	}
	if(len+1 > buffer_size){
		//printf("Not complete yet (%i)\n",len+1);
		//Packed Start correct, but not complete yet
		return 0;
	}

        if (buffer[len] == 0x0A || buffer[10] == 0x08) {
	    if(buffer[10] == 0x08){
	    	//fprintf(stdout,"Echosounder complete\n");
	    	return len;
	    }else{
            	return len+1;
	    }
        } else {
            //fprintf(stderr,"Message doesn't ended with an newline was(%u): %02X, skip message:\n",len,buffer[len]);
            //for (int i=0;i<buffer_size;i++)
            //    fprintf(stderr,"%02X ",buffer[i]);
	    //fprintf(stderr,"\n");
	    return -1;
        }
	printf("Hops darf nie passieren\n");
	//return -1;
}
