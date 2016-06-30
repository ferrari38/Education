//  A*algorithm
//
//  Created by Ohzawa Katsushi on 2015/12/27.
//  Copyright (c) 2015年 Ohzawa Katsushi. All rights reserved.
//


//A*アルゴリズム

#include <stdio.h>
#include <stdlib.h>

//グローバル宣言
void show_puzzle(char (*puzzle)[3]);
void up(char (*puzzle)[3]);
void down(char (*puzzle)[3]);
void right(char (*puzzle)[3]);
void left(char (*puzzle)[3]);
void show_list(struct list *p);
struct list *add_list_first(char (*puzzle)[3], struct list *parent, struct list *head);
struct list *add_list_last(char (*puzzle)[3], struct list *parent, struct list *head);
struct list *delete_list_first(struct list *head);
struct list *delete_list_last(struct list *head);
void addFn(struct list *p, char (*finish)[3]);
void cSort(struct list *head);

//リスト
struct list {
    char data[3][3];
    struct list *parent; //親節点のアドレス
    int fn; //評価関数
    struct list *next; //次のリストのアドレス
};

int main() {
    
    //パズルを生成
    char puzzle[3][3] = {{2, 8, 3}, {1, 6, 4}, {7, 0, 5}};
    show_puzzle(puzzle);
    
    //目標節点を生成
    char finish[3][3] = {{1, 2, 3}, {8, 0, 4}, {7, 6, 5}};
    
    //リストを生成
    struct list *lst1_head;
    lst1_head = NULL;
    struct list *lst2_head;
    lst2_head = NULL;
    
    printf("Serch Start\n");
    
    //step1　出発リストをlist1に格納
    lst1_head = add_list_first(puzzle, NULL, lst1_head);
    addFn(lst1_head, finish);
    
    while (1) {
        //step2 if L1 = 空　then 探索は失敗、終了
        if(lst1_head == NULL){
            printf("Search failed\n");
            return 0;
        }
        
        //step3 L1の先頭の節点L2を取り除きリストL2に入れる
        lst2_head = add_list_first(lst1_head->data, lst1_head->parent, lst2_head); //L2にpush
        lst1_head = delete_list_first(lst1_head); //L1の先頭リストを削除
        
        //step4 if nが目標節点である then 探索は成功、終了
        int i = 0;
        for(i=0; i<9; i++) {
            if(lst2_head->data[i/3][i%3] != finish[i/3][i%3])
                break;
        }
        if(i == 9){
            printf("Serch Finish\n\n");
            //結果を表示
            printf("L2(リスト２)を表示\n\n");
            show_list(lst2_head);
            
            return 0;
        }
        
        
        //step5 if nが展開できる（子接点を持つ）
        //      then 展開し、すべての子接点n[i]についてf'(n, n[i]) ← g'(n, n[i]) + h'(n[i])
        //      を計算する。ただし、g'(n, n[i])はSからnを通りn[i]に至るコストの評価値。
        //
        //      (i)n[i]がL1あるいはL2に含まれていないならば、f'(n[i]) ← f'(n, n[i])とし、n[i]をL1に入れ、
        //         nへのポイントを付ける。
        //      (ii)n[i]がL1に含まれており、f'(n, n[i]) < f'(n[i])ならば、
        //          f'(n[i]) ← f'(n, n[i])とし、nへのポインタを付ける。
        //      (iii)n[i]がL2に含まれており、f'(n, n[i]) < f'(n[i])ならば、
        //           f'(n[i]) ← f'(n, n[i])とし、n[i]をL2から取り除き、L1に入れ、
        //           nへのポインタを付ける。
        //
        //       L1内の節点をf'の昇順にソーティングする。step2へ
        //       else step2へ
        
        //0の位置を調べる
        int x, y, px, py;
        x = -1; y = -1; px = -1; py = -1;
        //子節点の存在する方向に節点を生成
        for(int i=0; i<3; i++) {
            for(int j=0; j<3; j++) {
                //親節点と比較
                if(lst2_head->data[i][j] == 0) {
                    y = i; x = j;
                }
                //親節点の親節点と比較
                if(lst2_head->parent != NULL) {
                    if(lst2_head->parent->data[i][j] == 0) {
                        py = i; px = j;
                    }
                }
            }
        }
        struct list *tmp;
        //left
        if(x != 0 && px != (x-1)) {
            if(lst1_head == NULL) {
                lst1_head = add_list_first(lst2_head->data, lst2_head, lst1_head);
                tmp = lst1_head;
            }
            else
                tmp = add_list_last(lst2_head->data, lst2_head, lst1_head);
            left(tmp->data);
            addFn(tmp, finish);
        }
        //right
        if(x != 2 && px != (x+1)) {
            if(lst1_head == NULL) {
                lst1_head = add_list_first(lst2_head->data, lst2_head, lst1_head);
                tmp = lst1_head;
            }
            else
                tmp = add_list_last(lst2_head->data, lst2_head, lst1_head);
            right(tmp->data);
            addFn(tmp, finish);
        }
        //up
        if(y != 0 && py != (y-1)) {
            if(lst1_head == NULL) {
                lst1_head = add_list_first(lst2_head->data, lst2_head, lst1_head);
                tmp = lst1_head;
            }
            else
                tmp = add_list_last(lst2_head->data, lst2_head, lst1_head);
            up(tmp->data);
            addFn(tmp, finish);
        }
        //down
        if(y != 2 && py != (y+1)) {
            if(lst1_head == NULL) {
                lst1_head = add_list_first(lst2_head->data, lst2_head, lst1_head);
                tmp = lst1_head;
            }
            else
                tmp = add_list_last(lst2_head->data, lst2_head, lst1_head);
            down(tmp->data);
            addFn(tmp, finish);
        }
        
        //L1をソーティング
        cSort(lst1_head);
        
    }
    
    return 0;
}

//パズルを表示する関数
void show_puzzle(char (*puzzle)[3]) {
    for(int i=0; i<3; i++) {
        printf("%d, %d, %d\n",puzzle[i][0], puzzle[i][1], puzzle[i][2]);
    }
    //改行
    printf("\n");
}

//空所を上に移動
void up(char (*puzzle)[3]) {
    int x = 0; //空所のx座標（横方向）
    int y = 0; //空所のy座標（縦方向）
    char tmp = 0; //変更前のパズルの値を覚える
    
    //空所の位置を取得
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            if(puzzle[i][j] == 0) {
                y = i; x = j;
            }
        }
    }
    
    //パズルの値を変更
    if(y != 0) {
        tmp = puzzle[y-1][x];
        puzzle[y-1][x] = 0;
        puzzle[y][x] = tmp;
    }
}

//空所を下に移動
void down(char (*puzzle)[3]) {
    int x = 0; //空所のx座標（横方向）
    int y = 0; //空所のy座標（縦方向）
    char tmp = 0; //変更前のパズルの値を覚える
    
    //空所の位置を取得
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            if(puzzle[i][j] == 0) {
                y = i; x = j;
            }
        }
    }
    
    //パズルの値を変更
    if(y != 2) {
        tmp = puzzle[y+1][x];
        puzzle[y+1][x] = 0;
        puzzle[y][x] = tmp;
    }
}

//空所を右に移動
void right(char (*puzzle)[3]) {
    int x = 0; //空所のx座標（横方向）
    int y = 0; //空所のy座標（縦方向）
    char tmp = 0; //変更前のパズルの値を覚える
    
    //空所の位置を取得
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            if(puzzle[i][j] == 0) {
                y = i; x = j;
            }
        }
    }
    
    //パズルの値を変更
    if(x != 2) {
        tmp = puzzle[y][((int)x+1)%3];
        puzzle[y][((int)x+1)%3] = 0;
        puzzle[y][x] = tmp;
    }
}

//空所を左に移動
void left(char (*puzzle)[3]) {
    int x = 0; //空所のx座標（横方向）
    int y = 0; //空所のy座標（縦方向）
    char tmp = 0; //変更前のパズルの値を覚える
    
    //空所の位置を取得
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            if(puzzle[i][j] == 0) {
                y = i; x = j;
            }
        }
    }
    
    //パズルの値を変更
    if(x != 0) {
        tmp = puzzle[y][x-1];
        puzzle[y][x-1] = 0;
        puzzle[y][x] = tmp;
    }
}

//リストの表示
void show_list(struct list *p)
{
    while (p != NULL) { //次ポインタがNULLまで処理
        show_puzzle(p->data);
        p = p->next;
    }
}

//リストの先頭にデータを登録
struct list *add_list_first(char (*puzzle)[3], struct list *parent, struct list *head)
{
    struct list *p;
    
    //記憶領域の確保
    if ((p = (struct list *) malloc(sizeof(struct list))) == NULL) {
        printf("malloc error\n");
        exit(EXIT_FAILURE);
    }
    
    // リストにデータを登録
    for(int i=0; i<9; i++) {
        p->data[i/3][i%3] = puzzle[i/3][i%3];
    }
    //リストに親節点のポインタを登録
    p->parent = parent;
    
    //ポインタのつなぎ換え
    p->next = head; //今までの先頭ポインタを次ポインタに
    head = p; //新たな領域を先頭ポインタに
    
    return head;
}

//リストの末尾にデータを登録
struct list *add_list_last(char (*puzzle)[3], struct list *parent, struct list *head)
{
    struct list *p;
    
    //記憶領域の確保
    if ((p = (struct list *) malloc(sizeof(struct list))) == NULL) {
        printf("malloc error\n");
        exit(EXIT_FAILURE);
    }
    
    // リストにデータを登録
    for(int i=0; i<9; i++) {
        p->data[i/3][i%3] = puzzle[i/3][i%3];
    }
    //リストに親節点のポインタを登録
    p->parent = parent;
    
    //リストを進める
    //ポインタのつなぎ換え
    if(head == NULL) {
        printf("L1 is NULL...\n");
        return 0;
    }
    else {
        struct list *tmp; tmp = head;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        //ポインタのつなぎ換え
        tmp->next = p; p->next = NULL;
    }
    
    return p;
}

//リストの先頭データを削除
struct list *delete_list_first(struct list *head)
{
    struct list *p;
    p = head->next; //先頭となるリストを代入
    free(head); //リストを解放
    
    return p;
}

//リストの末尾データを削除
struct list *delete_list_last(struct list *head)
{
    struct list *p;
    p = head;
    
    if(p->next == NULL) {
        p = NULL;
        free(head); //メモリ解放
    }
    else {
        //末尾となるリスト（現状の最後から２番目）まで進める
        while (p->next->next != NULL) {
            p = p->next; //ノードを進める
        }
        struct list *tmp;
        tmp = p->next;
        p->next = NULL;
        free(tmp);
    }
    
    return p;
}

//コストの評価値を代入する関数
void addFn(struct list *p, char (*finish)[3]) {
    int tmp = 0;
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            if(p->data[i][j] != finish[i][j]) {
                tmp = tmp + 1;
            }
        }
    }
    
    p->fn = tmp;
}

//昇順にソート
void cSort(struct list *head) {
    struct list *tmp1;
    struct list *tmp2;
    
    char tmp_data;
    struct list *tmp_parent;
    int tmp_fn;
    
    tmp1 = head;
    while (tmp1 != NULL) {
        tmp2 = tmp1->next;
        while (tmp2 != NULL) {
            if(tmp1->fn < tmp2->fn) {
                //データの入れ替え
                for (int i=0; i<3; i++) {
                    for (int j=0; j++; j++) {
                        tmp_data =  tmp1->data[i][j];
                        tmp1->data[i][j] = tmp2->data[i][j];
                        tmp2->data[i][j] = tmp_data;
                    }
                }
                
                tmp_parent = tmp1->parent;
                tmp1->parent = tmp2->parent;
                tmp2->parent = tmp_parent;
                
                tmp_fn = tmp1->fn;
                tmp1->fn = tmp2->fn;
                tmp2->fn = tmp_fn;
            }
            tmp2 = tmp2->next;
        }
        tmp1 = tmp1->next;
    }
}