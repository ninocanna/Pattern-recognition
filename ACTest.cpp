#include<cstdio>
#include<cstring>
#include<cmath>
#include<vector>
#include<windows.h>
using namespace std;

const int FEATURE_NUM = 40;
const int ITEM_NUM = 1000;


struct Item
{
    int type;
    double features[FEATURE_NUM];
} items[ITEM_NUM];


vector<int>chosenFeatures;
bool isChosen[FEATURE_NUM];
double value[FEATURE_NUM];

double result;
vector<int>resultSet;

double getDistance(Item a, Item b)
{
    double r = 0;
    for (int i = 0; i < chosenFeatures.size(); i++)
    {
        //printf("#%d",chosenFeatures[i]);
        r += (a.features[chosenFeatures[i]] - b.features[chosenFeatures[i]]) * (a.features[chosenFeatures[i]] - b.features[chosenFeatures[i]]);
    }
    //printf("#%f@\n",sqrt(r));
    return sqrt(r);
}

int findNearest(int id)
{
    double rmin;
    int r = -1;
    for (int i = 0; i < ITEM_NUM; i++)
    {
        if (i != id)
        {
            double dis = getDistance(items[id], items[i]);
            if (r == -1 || rmin > dis)
            {
                r = i;
                rmin = dis;
            }
        }
    }
    //printf("(%d,%d)",id,r);
    return r;
}

double getAccuracy()
{
    int num = 0;
    for (int i = 0; i < ITEM_NUM; i++)
    {
        if (items[findNearest(i)].type == items[i].type)
        {
            num++;
        }
    }
    return 1.0 * num / ITEM_NUM;
}

void output()
{
    printf("\nThe highest accuracy is %f.\n\n", result);
    printf("The chosen features are: \n{%d", resultSet[0]);
    for (int i = 1; i < resultSet.size(); i++)
    {
        printf(",%d", resultSet[i]);
    }
    printf("}\n\n");
}


void ini()
{
    chosenFeatures.clear();
    memset(isChosen, false, sizeof(isChosen));
    resultSet.clear();
    result = -1;
}

void forwardSelection()
{
    ini();
    for (int times = 1; times <= FEATURE_NUM; times++)
    {
        for (int i = 0; i < FEATURE_NUM; i++)
        {
            if (!isChosen[i])
            {
                chosenFeatures.push_back(i);
                value[i] = getAccuracy();
                //printf("~%f ",value[i]);
                chosenFeatures.pop_back();
            }
        }
        //printf("\n");
        int r = -1;
        double tmax ;
        for (int i = 0; i < FEATURE_NUM; i++)
        {
            if (!isChosen[i])
            {
                if (r == -1 || tmax < value[i])
                {
                    r = i;
                    tmax = value[i];
                }
            }
        }
        chosenFeatures.push_back(r);
        isChosen[r] = true;
        printf("~~~%f ", tmax);
        for (int i = 0; i < chosenFeatures.size(); i++)printf("%d ", chosenFeatures[i] + 1);
        printf("\n");
        if (tmax > result)
        {
            result = tmax;
            resultSet.clear();
            for (int i = 0; i < chosenFeatures.size(); i++)
            {
                resultSet.push_back(chosenFeatures[i] + 1);
            }
        }

    }
    output();
}

void deleteItem(int id)
{
    vector<int>::iterator it;
    for (it = chosenFeatures.begin(); it != chosenFeatures.end(); ++it)
    {
        if (*it == id)
        {
            chosenFeatures.erase(it);
            break;
        }
    }
}

void backwardElimination()
{
    ini();
    for (int i = 0; i < FEATURE_NUM; i++)
    {
        isChosen[i] = true;
        chosenFeatures.push_back(i);
    }
    for (int times = 1; times <= FEATURE_NUM; times++)
    {
        for (int i = 0; i < FEATURE_NUM; i++)
        {
            if (isChosen[i])
            {
                deleteItem(i);
                value[i] = getAccuracy();
                chosenFeatures.push_back(i);
            }
        }

        int r = -1;
        double m;
        for (int i = 0; i < FEATURE_NUM; i++)
        {
            if (isChosen[i])
            {
                if (r == -1 || m < value[i])
                {
                    r = i;
                    m = value[i];
                }
            }
        }

        isChosen[r] = false;
        deleteItem(r);

        printf("~~~%f ", m);
        for (int i = 0; i < chosenFeatures.size(); i++)printf("%d ", chosenFeatures[i] + 1);
        printf("\n");


        if (m > result)
        {
            result = m;
            resultSet.clear();
            for (int i = 0; i < chosenFeatures.size(); i++)
            {
                resultSet.push_back(chosenFeatures[i] + 1);
            }
        }
    }
    output();
}

bool avaliable[FEATURE_NUM];
vector<int> saveState;

void biDirection()
{
    ini();
    for (int i = 0; i < FEATURE_NUM; i++)
    {
        avaliable[i] = true;
    }
    double tmax;
    int r;
    for (int times = 1; times <= FEATURE_NUM; times++)
    {
        if (times & 1)
        {
            for (int i = 0; i < FEATURE_NUM; i++)
            {
                if (!isChosen[i] && avaliable[i])
                {
                    chosenFeatures.push_back(i);
                    value[i] = getAccuracy();
                    chosenFeatures.pop_back();
                }
            }

            r = -1;
            for (int i = 0; i < FEATURE_NUM; i++)
            {
                if (!isChosen[i] && avaliable[i])
                {
                    if (r == -1 || tmax < value[i])
                    {
                        r = i;
                        tmax = value[i];
                    }
                }
            }
            chosenFeatures.push_back(r);
            isChosen[r] = true;

            printf("~~~%f ", tmax);
            for (int i = 0; i < chosenFeatures.size(); i++)printf("%d ", chosenFeatures[i] + 1);
            printf("\n");

            if (tmax > result)
            {
                result = tmax;
                resultSet.clear();
                for (int i = 0; i < chosenFeatures.size(); i++)
                {
                    resultSet.push_back(chosenFeatures[i] + 1);
                }
            }

            saveState = chosenFeatures;

        }
        else
        {
            chosenFeatures.clear();
            for (int i = 0; i < FEATURE_NUM; i++)
            {
                if (avaliable[i])
                {
                    chosenFeatures.push_back(i);
                }
            }
            for (int i = 0; i < FEATURE_NUM; i++)
            {
                if (!isChosen[i] && avaliable[i])
                {
                    deleteItem(i);
                    value[i] = getAccuracy();
                    chosenFeatures.push_back(i);
                }
            }

            r = -1;
            for (int i = 0; i < FEATURE_NUM; i++)
            {
                if (!isChosen[i] && avaliable[i])
                {
                    if (r == -1 || tmax < value[i])
                    {
                        r = i;
                        tmax = value[i];
                    }
                }
            }

            avaliable[r] = false;
            deleteItem(r);

            printf("~~~%f ", tmax);
            for (int i = 0; i < chosenFeatures.size(); i++)printf("%d ", chosenFeatures[i] + 1);
            printf("\n");

            if (tmax > result)
            {
                result = tmax;
                resultSet.clear();
                for (int i = 0; i < chosenFeatures.size(); i++)
                {
                    resultSet.push_back(chosenFeatures[i] + 1);
                }
            }
            chosenFeatures = saveState;
        }

    }
    output();
}


/*****************/

bool used[FEATURE_NUM];

void dfs(int ind)
{
    if(ind>=FEATURE_NUM)
    {
        chosenFeatures.clear();
        for(int i=0;i<FEATURE_NUM;i++)
        {
            if(used[i])
            {
                chosenFeatures.push_back(i);
                //printf("%d ",i+1);
            }
        }
        double tres = getAccuracy();
        //printf("Accuracy: %f \n",tres);
        if(tres>result)
        {
            result = tres;
            resultSet = chosenFeatures;
        }
    }
    else
    {
        used[ind]=true;
        dfs(ind+1);
        used[ind]=false;
        dfs(ind+1);
    }
}

void bruteForce()
{
    ini();
    memset(used,false,sizeof(used));
    dfs(0);
    output();
}

/*****************/

int main()
{

    freopen("cs_170_large19.txt", "r", stdin);
    //system("pause");
    for (int i = 0; i < ITEM_NUM; i++)
    {
        double type;
        scanf("%lf", &type);
        items[i].type = (int)type;
        for (int j = 0; j < FEATURE_NUM; j++)
        {
            scanf("%lf", &items[i].features[j]);
            // printf("%f ", items[i].features[j]);
        }
        //printf("\n");
    }
    printf("Forward Selection: \n");
    forwardSelection();
    printf("Backward Elimination: \n");
    backwardElimination();
    printf("Bi-direction: \n");
    biDirection();
    printf("Brute Force: ");
    bruteForce();
    return 0;
}
