#include "configdata.h"
#include <QDebug>

ConfigData::ConfigData()
{

}

ConfigData::~ConfigData()
{
    //delete list cache
}

int ConfigData::initConf()
{
    confCache = loadConf();
    if(!confCache) return -1;
    return 0;
}

QList<KeyValue *> *ConfigData::loadConf()
{
    char* iscsHome;
    char filePath[1024];
    FILE* fp;

    iscsHome = getenv("ISCS_HOME");
    if(iscsHome){
        sprintf(filePath,"%s/%s",iscsHome,CONF_FILE);
    }else{
        printf("env ISCS_HOME isn't exist!\n");
        return NULL;
    }
//    delete iscsHome;

    printf("load file,path - %s\n",filePath);
    if(!(fp = fopen(filePath,"r"))){
        printf("file open failed!\n");
    }

    char buf[1024] = {0};
    QList<KeyValue*> * keyValueList = new QList<KeyValue*>();
    while (fgets(buf,sizeof(buf),fp)) {
//        printf("buf - %s\n",buf);
        qDebug("buf - %s",buf);
        if('#' != buf[0] && '\n' != buf[0]){
            QString str(buf);
            str.trimmed();
            QStringList list;
            list = str.split(" ");
            printf("list len - %d\n",list.size());
            if(list.size() != 2){
                printf("list len isn't 2!\n");
                continue;
            }
            QString qKey = list.value(0);//T QList::value(int i) const
            QString qValue = list.value(1);
            qDebug()<<"qKey - "<<qKey<<"qValue - "<<qValue;

            char* key = NULL;
            char* value = NULL;
            key = qKey.toLocal8Bit().data();
            value = qValue.toLocal8Bit().data();
//            printf("key - %s,value - %s\n",key,value);
//            printf("key - %p %s,value - %p %s\n",key,key,value,value);

            //for loop
            char* data = NULL;
            KeyValue* keyValue = new KeyValue();
            for(int i = 0;i < list.size();i++){
                QString temp = list.value(i);
                QByteArray ba = temp.toLocal8Bit();
                data = ba.data();
//                data = temp.toLocal8Bit().data();
//                printf("data - %p %s\n",data,data);
                if(0 == i){
//                    keyValue->key = data;
                    keyValue->key = strdup(data);
                }
                if(1 == i){
//                    keyValue->value = data;
                    keyValue->value = strdup(data);
                }
            }
            keyValueList->append(keyValue);
        }

        memset(buf,'\0',1024);
    }
    fclose(fp);
    return keyValueList;
}

int ConfigData::saveConf(QList<KeyValue *> *keyValues)
{
    char* iscsHome;
    char filePath[1024];
    FILE* fp;
    char filePathOut[1024];
    FILE* fpOut;

    iscsHome = getenv("ISCS_HOME");
    if(iscsHome){
        sprintf(filePath,"%s/%s",iscsHome,CONF_FILE);
        sprintf(filePathOut,"%s/%s",iscsHome,CONF_FILE_OUT);
    }else{
        printf("env ISCS_HOME isn't exist!\n");
        return -1;
    }
//    delete iscsHome;

    printf("load file,path - %s\n",filePath);
    if(!(fp = fopen(filePath,"r"))){
        printf("file open failed!\n");
    }
    printf("load file,path - %s\n",filePathOut);
    if(!(fpOut = fopen(filePathOut,"w+"))){
        printf("file open failed!\n");
    }

    char buf[1024] = {0};
    int index = 0;
    while(fgets(buf,sizeof(buf),fp)){
        printf("buf - %s\n",buf);
        if('#' == buf[0] || '\n' == buf[0] || '\r' == buf[0]){
            int putsRet = fputs(buf,fpOut);
            if(putsRet < 0){
                printf("fputs error!\n");
            }
        }else{
            printf("sprintf keyValues,index - %d\n",index);
            char keysBuf[1024] = {0};
            if(index >= keyValues->size()){
                printf("out of keyValues list index!\n");
                return -2;//file keyValues size isn't match list
            }
            KeyValue* temp = keyValues->value(index);
            printf("key - %s,value - %s\n",temp->key,temp->value);
            sprintf(keysBuf,"%s %s\n",temp->key,temp->value);

            int putsRet = fputs(keysBuf,fpOut);
            if(putsRet < 0){
                printf("fputs error!\n");
            }

            index++;
        }
    }

    printf("start close files!\n");
    fclose(fp);
    fclose(fpOut);
    printf("end close files!\n");

    //rename
    int removeRet = remove(filePath);
    if(!removeRet){
        printf("old file remove success!\n");
    }
    int renameRet = rename(filePathOut,filePath);
    if(!renameRet){
        printf("rename file success!\n");
    }
    return 0;
}

const char *ConfigData::getValueByKey(const char *key)
{
    foreach (KeyValue* temp, *confCache) {
        if(!strcmp(temp->key,key)){
            const char* value = NULL;
            value = temp->value;
            return value;
        }
    }
    return NULL;
}
