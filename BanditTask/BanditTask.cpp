//  BanditTask
//
//  Created by Ohzawa Katsushi on 2016/01/31.
//  Copyright (c) 2016年 Ohzawa Katsushi. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

double rand_normal(double mu, double sigma);
double Uniform();

class Task {
    
private:
    
    //スロットマシーン（10台）の平均
    double slotMu[10];
    
    //各回数で選んだスロットの番号
    int slotNum[1000];
    
    //スロットを回すメソッド
    //引数 : スロットの番号
    double slotPlay(int num) {
        return rand_normal(slotMu[num], 1);
    }
    
    
public:
    
    //各回数での結果
    double result[1000];
    
    //コンストラクタ
    Task() {
        //スロットマシーン（10台）の平均値を決定
        for (int i=0; i<10; i++) {
            slotMu[i] = rand_normal(0, 1);
        }
    }
    
    //グリーディー法でスロットをプレイ（1000回）
    void taskPlay() {
        int count; //プレイ回数
        int slotCount[10] = {0}; //各スロットを回した回数
        double sum[10] = {0.0}; //各スロットの報酬の合計
        int maxNum = -1; //報酬の平均の最大値を出したスロット番号
        
        //１番最初に回すスロットを選ぶ
        slotNum[0] = rand()%10;
        
        //1000回プレイする
        for(count = 0; count<1000; count++) {
            //printf("count : %d\n", count);
            
            //スロットをプレイ
            result[count] = slotPlay(slotNum[count]);
            slotCount[slotNum[count]]++;
            
            //プレイしたスロットの報酬の合計を更新
            sum[slotNum[count]] += result[count];
            
            //printf("result[%d] : (%d, %f)\n", count, slotNum[count], result[count]);
            //printf("sum : %f\n", sum[slotNum[count]]);
            
            
            //最も良いスロット番号を更新
            maxNum = slotNum[count];
            for (int i=0; i<10; i++) {
                if(slotCount[i] != 0 && sum[maxNum]/slotCount[maxNum] < sum[i]/slotCount[i]) {
                    maxNum = i;
                }
                else if(slotCount[i] == 0 && sum[maxNum]/slotCount[maxNum] < 0) {
                    maxNum = i;
                }
            }
            //次に回すスロットを決定
            if(count < 999) {
                slotNum[count+1] = maxNum;
            }
            
            //printf("nextSlot : %d\n\n", maxNum);
        }
    }
    
    //εグリーディー法でスロットをプレイ（1000回）
    //引数 : ε
    void etaskPlay(double e) {
        int count; //プレイ回数
        int slotCount[10] = {0}; //各スロットを回した回数
        double sum[10] = {0.0}; //各スロットの報酬の合計
        int maxNum = -1; //報酬の平均の最大値を出したスロット番号
        
        //1000回プレイする
        for(count = 0; count<1000; count++) {
            //printf("count : %d\n", count);
            
            //ランダムにスロットをプレイ
            if(rand()%(int)(1/e) == 0) {
                //スロットをプレイ
                slotNum[count] = rand()%10;
                result[count] = slotPlay(slotNum[count]);
                slotCount[slotNum[count]]++;
                
            }
            else {
                //スロットをプレイ
                result[count] = slotPlay(slotNum[count]);
                slotCount[slotNum[count]]++;
                
            }
            
            //プレイしたスロットの報酬の合計を更新
            sum[slotNum[count]] += result[count];
            
            //printf("result[%d] : (%d, %f)\n", count, slotNum[count], result[count]);
            //printf("average : %f\n", sum[slotNum[count]]/slotCount[slotNum[count]]);
            
            
            //最も良いスロット番号を更新
            maxNum = slotNum[count];
            for (int i=0; i<10; i++) {
                if(slotCount[i] != 0 && sum[maxNum]/slotCount[maxNum] < sum[i]/slotCount[i]) {
                    maxNum = i;
                }
                else if(slotCount[i] == 0 && sum[maxNum]/slotCount[maxNum] < 0) {
                    maxNum = i;
                }
            }
            //次に回すスロットを決定
            if(count < 999) {
                slotNum[count+1] = maxNum;
            }
            
            //printf("nextSlot : %d\n\n", maxNum);
        }
    }
    
};


int main() {
    
    //乱数を初期化
    srand((unsigned int)time(0));
    
    //タスクを実行
    Task *t;
    t = new Task[2000];
    
    for (int i=0; i<2000; i++) {
        //グリーディー法
        //t[i].taskPlay();
        //eグリーディー法
        //t[i].etaskPlay(0.10);
        t[i].etaskPlay(0.010);
    }
    
    ////////////////////
    //結果をグラフへ書き出し
    ////////////////////
    
    FILE *p; //ファイルポインタ
    p = fopen("tasks0.01.xls","w");
    
    if(p==NULL){
        printf("Can't open file\n");
        exit (1);
    }
    
    double tmp;
    
    for(int i=0; i<1000; i++) {
        tmp = 0;
        for(int j=0; j<2000; j++) {
            tmp += t[j].result[i];
        }
        
        tmp = tmp / 2000;
        
        fprintf(p, "%f\n", tmp);
    }
    
    fclose(p);
    
    printf("Exel にファイルを書き出しました。\n");
    printf("Exel内でグラフを作成してください。\n");
    
    //メモリ解放
    free(t);
    

    return 0;
}


//正規分布に従い乱数を発生させる
double rand_normal(double mu, double sigma){
    double z = sqrt(-2.0 * log(Uniform())) * sin(2.0 * M_PI * Uniform());
    return mu + sigma*z;
}

//一様分布に従い乱数を発生させる（0から1）
double Uniform(){
    return ((double)rand()+1.0)/((double)RAND_MAX+2.0);
}