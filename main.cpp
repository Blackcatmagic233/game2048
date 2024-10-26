
#include "allhe.h"


using namespace std;
using ll = long long;
using db = long double;

//颜色参数
enum Color
{
    zero = RGB(205,193,180),
    t1 = RGB(238,228,218),
    t2 = RGB(237,224,200),
    t3 = RGB(242,177,121),
    t4 = RGB(245,149,99),
    t5 = RGB(246,124,95),
    t6 = RGB(246,94,59),
    t7 = RGB(242,177,121),
    t8 = RGB(237,204,97),
    t9 = RGB(255,0,128),
    t10 = RGB(145,0,72),
    t11 = RGB(242,17,158),
    t12 = RGB(111,111,11),
    t13 = RGB(111,111,11),
    t14 = RGB(111,111,11),
    back = RGB(161,169,208),
};
Color arr1[16] = {zero,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,back};


/*
格子的参数：
    格子宽：100
    最大格子数 4
    格子间距 15

*/
const int GRID_W = 100;
const int MAX_SIZE = 4;
const int INTERVAL = 15;

//分数
ll score = 0;


//数据图
vector<vector<int>>  mp (MAX_SIZE,vector<int>(MAX_SIZE,0));
bool hasMoved = false;

//屏幕输出的字符
char str2[20] ;

//难度参数
bool rt = 0;

//游戏次数
int t = 1;


string playerName;                          //  玩家名称
const string RANK_FILE = "rank.txt";        //  排行榜路径
const string SAVE_FILE = "save_game.txt";   //  存档文件路径

// 保存游戏的当前状态到文件
/*************************************************
 * Function: saveGame()
 * Description: 保存游戏
 * Calls: None
 * Called By: main
 * Input: 分数score,地图mp;
 * Output:  SAVE_FILE
 * Return: None
*************************************************/

class SAVE{
public:
    void saveGame()
    {
        ofstream fout(SAVE_FILE);
        if (fout.is_open())
        {
            fout << score << endl;  // 保存当前分数
            for (int i = 0; i < MAX_SIZE; ++i)
            {
                for (int j = 0; j < MAX_SIZE; ++j)
                {
                    fout << mp[i][j] << " ";  // 保存棋盘状态
                }
                fout << endl;
            }
            fout.close();
            cout << "Game saved" << endl;
        }
        else
        {
            cout << "Failed to save" << endl;
        }
    }


// 从存档文件中读取游戏状态
/*************************************************
Function: // loadGame()
Description: // 加载先前保存的游戏
Calls: //     None
Called By: // GameInit()
Input: //     None
Output: //    None
Return: //    先前分数score,地图mp
Others: // 无
*************************************************/


};

class SAV : public SAVE{
private:
    map<int,int> mp;
};


class RUN{

public:

    bool loadGame()
    {
        ifstream fin(SAVE_FILE);
        if (fin.is_open())
        {
            fin >> score;  // 读取分数
            for (int i = 0; i < MAX_SIZE; ++i)
            {
                for (int j = 0; j < MAX_SIZE; ++j)
                {
                    fin >> mp[i][j];  // 读取棋盘状态
                }
            }
            fin.close();
            return true;
        }
        return false;
    }




// 保存玩家分数到文件
/*************************************************
Function: // saveScore()
Description: // 保存玩家分数到文件
Calls: //     None
Called By: // GameInit()
Input: //     None
Output: //    None
Return: //    先前分数score,地图mp
Others: // 无
*************************************************/
// 创建一个ofstream对象fout，用于向文件写入数据。这里指定了文件名（RANK_FILE）和打开模式（ios::app）。
// ios::app表示以追加模式打开文件，即写入的数据会被添加到文件内容的末尾，而不是覆盖原有内容。
    void saveScore(const string& playerName1, ll score1)
    {
        ofstream fout(RANK_FILE, ios::app);
        // 检查文件是否成功打开。
        if (fout.is_open())
        {
            // 如果文件成功打开，则使用fout向文件写入数据。
            // 这里写入的是玩家名称（playerName1）和分数（score1），两者之间用空格分隔，最后写入一个换行符\n。
            // 这样，每次调用这段代码时，都会将新的玩家名称和分数追加到RANK_FILE文件的末尾。
            fout << playerName1 << " " << score1 << '\n';
            fout.close();
        }
    }

// 链表节点结构体
    struct RankNode {
        string name;
        ll score1;
        RankNode* next;
        RankNode(string n, ll s) : name(n), score1(s), next(nullptr) {}
    };

// 读取排行榜并返回排序后的链表
    RankNode* getRankList() {
        ifstream fin(RANK_FILE);
        RankNode* head = nullptr;

        if (fin.is_open()) {
            string name;
            ll playerScore;

            // 读取文件并插入到链表中（按分数降序插入）
            while (fin >> name >> playerScore) {
                RankNode* newNode = new RankNode(name, playerScore);
                // 如果链表为空，直接设置为头节点
                if (!head) {
                    head = newNode;
                } else {
                    // 找到合适位置插入，保持链表按分数从大到小排序
                    RankNode* prev = nullptr;
                    RankNode* current = head;
                    while (current && current->score1 > playerScore) {
                        prev = current;
                        current = current->next;
                    }

                    if (!prev) {
                        // 插入到头部
                        newNode->next = head;
                        head = newNode;
                    } else {
                        // 插入到链表中间或尾部
                        prev->next = newNode;
                        newNode->next = current;
                    }
                }
            }
            fin.close();
        }

        return head;
    }
    void displayRank(int sum_size)
    {
        RankNode* rankList = getRankList();

        cleardevice();
        outtextxy(100, 50, "Rank List:");

        // 显示前10名玩家
        RankNode* current = rankList;
        for (int i = 0; i < 10 && current; i++)
        {
            char rankStr[50];
            sprintf(rankStr, "%d. %s: %lld", i + 1, current->name.c_str(), current->score1);
            outtextxy(100, 100 + i * 50, rankStr);
            current = current->next;
        }

        // 显示分数并等待用户按键继续
        outtextxy(1, sum_size - 50, "Press any key to continue...");
        _getch();

        // 释放链表内存
        while (rankList) {
            RankNode* temp = rankList;
            rankList = rankList->next;
            delete temp;
        }
    }







    // 随机数函数 ：随机产生2/4 概率9 : 1
int RandNum()
{

    srand((unsigned)time(nullptr) + rand() * clock());
    int TimeTmp = rand() % 10;

    if(TimeTmp ==  9) return 4;
    else return 2;

}

//移动逻辑
void MoveUp() {
    for (int i = 0; i < MAX_SIZE; i++) {
        int temp = 0; // 指向可放置的位置
        for (int begin = 0; begin < MAX_SIZE; begin++) {
            if (mp[begin][i] != 0) { // 当前方块不为空
                if (temp == 0 || mp[temp - 1][i] != mp[begin][i]) {
                    // 如果目标位置为空，或目标位置与当前不同
                    if (temp != begin) {
                        mp[temp][i] = mp[begin][i]; // 移动方块
                        mp[begin][i] = 0; // 清空原位置
                        hasMoved = true;  // 标记有移动
                    }
                    temp++; // 移动位置指针
                } else {
                    // 如果目标位置与当前方块相同，进行合并
                    mp[temp - 1][i] += mp[begin][i];
                    mp[begin][i] = 0; // 清空原位置
                    hasMoved = true;  // 标记有合并
                    score+=mp[begin][i];

                }
            }
        }
    }
}

void MoveDown() {
    for (int i = 0; i < MAX_SIZE; i++) {
        int temp = MAX_SIZE - 1; // 指向可放置的位置
        for (int begin = MAX_SIZE - 1; begin >= 0; begin--) {
            if (mp[begin][i] != 0) {
                if (temp == MAX_SIZE - 1 || mp[temp + 1][i] != mp[begin][i]) {
                    if (temp != begin) {
                        mp[temp][i] = mp[begin][i];
                        mp[begin][i] = 0;
                        hasMoved = true;  // 标记有移动
                    }
                    temp--; // 移动位置指针
                } else {
                    mp[temp + 1][i] += mp[begin][i];
                    mp[begin][i] = 0;
                    hasMoved = true;  // 标记有移动
                    score+=mp[begin][i];

                }
            }
        }
    }
}

void MoveLeft() {
    for (int i = 0; i < MAX_SIZE; i++) {
        int temp = 0; // 指向可放置的位置
        for (int begin = 0; begin < MAX_SIZE; begin++) {
            if (mp[i][begin] != 0) {
                if (temp == 0 || mp[i][temp - 1] != mp[i][begin]) {
                    if (temp != begin) {
                        mp[i][temp] = mp[i][begin];
                        mp[i][begin] = 0;
                        hasMoved = true;  // 标记有移动
                    }
                    temp++;
                } else {
                    mp[i][temp - 1] += mp[i][begin];
                    mp[i][begin] = 0;
                    hasMoved = true;  // 标记有移动
                    score+=mp[i][begin];

                }
            }
        }
    }
}

void MoveRight() {
    for (int i = 0; i < MAX_SIZE; i++) {
        int temp = MAX_SIZE - 1; // 指向可放置的位置
        for (int begin = MAX_SIZE - 1; begin >= 0; begin--) {
            if (mp[i][begin] != 0) {
                if (temp == MAX_SIZE - 1 || mp[i][temp + 1] != mp[i][begin]) {
                    if (temp != begin) {
                        mp[i][temp] = mp[i][begin];
                        mp[i][begin] = 0;
                        hasMoved = true;  // 标记有移动
                    }
                    temp--; // 移动位置指针
                } else {
                    mp[i][temp + 1] += mp[i][begin];
                    mp[i][begin] = 0;
                    score+=mp[i][begin];
                    hasMoved = true;  // 标记有移动
                }
            }
        }
    }
}



//按键反馈
void KeyF()
{
    //获取键盘按键
    char key1 = _getch();
    switch (key1)
    {
        case 'w':
        case 'W':
        case 72:
            MoveUp();
            break;
        case 's':
        case 'S':
        case 80:
            MoveDown();
            break;
        case 'a':
        case 'A':
        case 75:
            MoveLeft();
            break;
        case 'd':
        case 'D':
        case 77:
            MoveRight();
            break;
    }
}

//初始化数据_两个数
//初始化数据_两个数
void GameInit()
{
    cout << "Load previous game? (Y/N): ";
    char loadChoice;
    cin >> loadChoice;
    while(!(loadChoice == 'Y' || loadChoice == 'y'|| loadChoice == 'N'|| loadChoice == 'n') ){
        cerr << "error" << '\n' << "plz,retry" <<'\n';
        cin >> loadChoice;
    }
    if (loadChoice == 'Y' || loadChoice == 'y')
    {
        if (!loadGame())
        {
            cout << "No saved game found, starting a new game." << endl;
            // 没有存档，生成新棋盘
        }
        else
        {
            cout << "Game loaded successfully." << endl;
        }
    }

    // 无论是否加载存档，继续生成新棋盘
    for (int i = 0; i < 2;)
    {
        int l = rand() % MAX_SIZE;
        int r = rand() % MAX_SIZE;
        if (mp[l][r] == 0)
        {
            mp[l][r] = RandNum();
            i++;
        }
    }
}


//随机生成一个数
void GameInit2()
{
        while(1)
        {
            int l = rand() % MAX_SIZE;
            int r = rand() % MAX_SIZE;
            if(mp[l][r] == 0)
            {
                mp[l][r] = RandNum();
                break;
            }
        }

}

//退出函数
bool exit()
{
    for(int i = 0;i < MAX_SIZE;i++)
    {
        for(int j = 0;j < MAX_SIZE;j++)
        {
            if(mp[i][j]==0)return false;
        }
    }
    for(int i = 0 ;i < MAX_SIZE ;i++){
        for(int j = 0;j < MAX_SIZE ;j++){
            if(i == 0 || i == MAX_SIZE - 1 ){
                if(mp[i][j] == mp[i][j-1] ||mp[i][j] == mp[i][j+1]){
                    return false;
                }
            }
            if(j == 0 || j == MAX_SIZE - 1) {
                if (mp[i][j] == mp[i - 1][j] || mp[i][j] == mp[i + 1][j]) {
                    return false;
                }
            }
            if(mp[i][j] == mp[i-1][j] ||mp[i][j] == mp[i][j-1] || mp[i][j] == mp[i+1][j] ||mp[i][j] == mp[i][j+1]  ){
                return false;
            }
        }
    }
    return  true;
}

// 清除上一次的打印
void clearScreen() {
    // 使用换行清除之前的输出.
    cout << string(50, '\n');  // 简单的输出多行来覆盖之前的输出
}

// 打印当前棋盘
void printBoard() {
    clearScreen();  // 每次打印前清除屏幕
    cout << "Current Board:" << endl;
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            if (mp[i][j] == 0) {
                // 如果是 0，输出空白
                cout << setw(5) << left << " ";
            } else {
                // 否则输出数字，并确保对齐
                cout << setw(5) << left << mp[i][j];
            }
        }
        cout << endl;
    }
}


//创建游戏
void GameCreate()
{

    //背景颜色
    setbkcolor(RGB(161,169,208));
    cleardevice();

    //绘制内部实心矩形格子
    for(int i = 0;i < MAX_SIZE;i++)
    {
        for(int j = 0;j < MAX_SIZE;j++)
        {
            //左上角与右下角坐标
            int x = j * GRID_W + (j+1) * INTERVAL;
            int y = i * GRID_W + (i+1) * INTERVAL;



            //设置格子颜色
            //log2(0)未定义，特判
            int index = mp[i][j] == 0 ? 0 : (int)log2(mp[i][j]);
            COLORREF TColor = arr1[index];
            setfillcolor(TColor);

            //绘制格子
            solidroundrect(x,y,x+GRID_W,y+GRID_W,15,15);

            if(mp[i][j] != 0)
            {
                //文字参数
                settextstyle(40,0,"黑体");

                //文字背景透明
                setbkmode(TRANSPARENT);
                settextcolor(RGB(119,110,101));

                //数字转字符
                char str[10] = "";
                sprintf(str,"%d",mp[i][j]);

                // 字体位置居中
                int tw = textwidth(str);
                int th = textheight(str);
                int tx = (GRID_W - tw) / 2;
                int ty = (GRID_W - th) / 2;

                outtextxy(x + tx,y + ty,str);

                int sum_size = MAX_SIZE * GRID_W + INTERVAL * (MAX_SIZE + 1);

                sprintf(str2,"%lld",score);
                outtextxy(sum_size/5,sum_size+20,"score:");
                outtextxy(sum_size/2,sum_size+20,str2);
            }


            printBoard();
        }

    }
}

//常规胜利检测函数
bool winn()
{
    for(int i = 0;i < MAX_SIZE;i++)
    {
        for(int j = 0;j < MAX_SIZE;j++)
        {
            if(mp[i][j]==2048)return true;
        }
    }
    return false;
}

//重置mp
void reset()
{
    for(int i = 0; i < MAX_SIZE; i++)
    {
        for(int j = 0; j < MAX_SIZE; j++)
        {
            mp[i][j] = 0;
        }
    }
}




void solve ()
{
    cout << "Enter player name: ";
    cin >> playerName;  // 输入玩家名称
    //创建图形窗口
    int sum_size = MAX_SIZE * GRID_W + INTERVAL * (MAX_SIZE + 1);
    initgraph(sum_size,sum_size + 100);

    //调用数据
    GameInit();

    //内部界面创建

    while(1)
    {
        GameCreate();
        KeyF();//按键逻辑

        if (hasMoved)
        {
            GameInit2(); // 只有在移动发生时，才生成新方块
            hasMoved = false; // 重置标记
        }
        if(winn())
        {
            if(rt!=1)
            {
                // 游戏胜利，保存分数并显示排行榜
                saveScore(playerName, score);
                displayRank(sum_size);  // 显示排行榜


                cleardevice();
                outtextxy(sum_size/3,sum_size/3 - 70,"You Win");
                outtextxy(sum_size/5-60,sum_size/3 + 70,"try again (press -)");
                outtextxy(sum_size/5-60,sum_size/3 + 140,"play BOOST (press =)");
                outtextxy(sum_size/5,sum_size+20,"score:");
                outtextxy(sum_size/2,sum_size+20,str2);
                bool tpp = 0;
                while(tpp == 0)
                {
                    char key1 = _getch();
                    switch (key1)
                    {
                        case '-':
                        case '_':
                            rt = false;
                            tpp = 1;
                            t++;
                            reset();
                            cleardevice();
                            return;
                        case '+':
                        case '=':
                            rt = true;
                            tpp = 1;
                            t++;
                            reset();
                            cleardevice();
                            return;
                    }
                }
            }

        }
        if(exit())
        {
            // 游戏结束，保存分数并显示排行榜
            saveScore(playerName, score);
            displayRank(sum_size);  // 显示排行榜


            cleardevice();
            outtextxy(sum_size/3,sum_size/3 - 70,"You Lost");
            outtextxy(sum_size/5-60,sum_size/3 + 70,"try again (press -)");
            outtextxy(sum_size/5-60,sum_size/3 + 140,"play BOOST (press =)");
            outtextxy(sum_size/5,sum_size+20,"score:");
            outtextxy(sum_size/2,sum_size+20,str2);
            bool tpp = 0;
            while(tpp == 0)
            {
                char key1 = _getch();
                switch (key1)
                {
                    case '-':
                    case '_':
                        rt = false;
                        tpp = 1;
                        t++;
                        reset();
                        cleardevice();
                        return;
                    case '+':
                    case '=':
                        rt = true;
                        tpp = 1;
                        t++;
                        reset();
                        cleardevice();
                        return;
                }
            }


//            break;
        }
    }


    closegraph();// 关闭窗口


}
//void startGame();  // 声明开始游戏函数
//void exitGame();   // 声明退出游戏函数

// 主界面函数
void showMenu() {
    initgraph(400, 300);  // 初始化图形窗口，宽400，高300
    setbkcolor(WHITE);
    cleardevice();  // 清屏

    settextstyle(30, 0, "Consolas");
    setbkmode(TRANSPARENT);
    settextcolor(RGB(119,110,101));
    outtextxy(100, 80, "1. Start");
    outtextxy(100, 160, "2. Exit");

    while (true)
    {
        char choice = _getch();  // 获取用户输入
        if (choice == '1') {
            cleardevice();  // 关闭当前界面
            startGame();   // 调用开始游戏函数
            break;
        } else if (choice == '2') {
            closegraph();  // 关闭当前界面
            exitGame();    // 调用退出游戏函数
            break;
        }
    }
}


// 开始游戏函数
void startGame()
{
    // 2048 游戏逻辑代码
    // 调用 solve() 函数或其他与游戏相关的逻辑
    cout << "GameStart!" << '\n';
    // 这里可以直接调用现有的 solve() 函数
    outtextxy(100, 80, "input ur name (under)");
    solve();
}

// 退出游戏函数
void exitGame() {
    cout << "ExitGame..." << '\n';
    ::exit(0);  // 结束程序
}

};
int main ()
{
//    ios::sync_with_stdio(false),cin.tie(nullptr),cout.tie(nullptr);
//    cin >> t;
    SetConsoleOutputCP(CP_UTF8);
    RUN r1;
    SAVE s1;
    while(t--){
        r1.showMenu();  // 调用主界面函数
        // 在退出时询问是否存档
        cout << "Do you want to save the game? (Y/N): \n";
        cout << "(shi bai bu suan)" << '\n';
        char saveChoice;
        cin >> saveChoice;
        if (saveChoice == 'Y' || saveChoice == 'y') {
            s1.saveGame();  // 保存游戏
        }
    }



    return 0;
}