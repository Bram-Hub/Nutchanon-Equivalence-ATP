#include"iostream"
#include"fstream"
#include"algorithm"
#include"string"
#include"set"
#include"vector"
using namespace std;
/*
format: and = && =1, 
        or = || = 2, 
        if...then = -> = 3, 
        iff = <-> = 4, 
        not = ! =5,
        True = T = 8,
        False = F = 9, 
        atomic variable = character (restricted to only a-z or A-Z)
        ( ) stay the same
        format for output = only wff 
*/

string template1[] ={"!","&&","||","->","<->","","","","T","F"};

bool check_wff(string temp)
{
    int len = temp.length();
    int count =0; // count ( and ) if it's negative = not wff
    int a_c[100];
    for(int i=0;i<99;i++) a_c[i]=0;
    for(int i=0;i<len;i++)
    {
        if(temp[i]=='(')
        { count++; }
        else if(temp[i]==')')
        { 
            count--;
            if( count <0) return false; 
            a_c[count+1]=0;
        }
        else if((temp[i]>='a'&&temp[i]<='z')||(temp[i]>='A'&&temp[i]<='Z')||temp[i]=='8'||temp[i]=='9')
        {
            if(i!=len-1 && ((temp[i+1]>='a'&&temp[i+1]<='z')||(temp[i+1]>='A'&&temp[i+1]<='Z')||temp[i+1]=='8'||temp[i+1]=='9') )
                return false;
            if(i!=len-1 && temp[i+1]=='0')
                return false;
        }
        else if(temp[i]=='0')
        {
            if(i==len-1) 
                return false;
            if(temp[i+1]=='0'||temp[i+1]=='('||( (temp[i+1]>='a'&&temp[i+1]<='z')||(temp[i+1]>='A'&&temp[i+1]<='Z')||temp[i+1]=='8'||temp[i+1]=='9'))
            {}
            else
                return false;
        }
        else if(temp[i]>='1'&&temp[i]<='4')
        {
            if(a_c[count]==0) a_c[count]+=(temp[i]-'0');
            else if(a_c[count]==1||a_c[count]==2)
            {
                if(a_c[count]!=(temp[i]-'0'))
                    return false;
            }
            else
                return false;
        }
        else
        {
            return false;
        }
    }

    if(count>0) 
        return false;
    return true;
}

string transform(string temp)
{
    string str="";
    int len = temp.length();
    for(int i=0;i<len;i++)
    {
        if(temp[i]=='&')
        { str+="1"; i++; }
        else if(temp[i]=='|')
        { str+="2"; i++; }
        else if(temp[i]=='-')
        { str+="3"; i++; }
        else if(temp[i]=='<')
        { str+="4"; i+=2; }
        else if(temp[i]=='T')
        { str+="8"; }
        else if(temp[i]=='F')
        { str+="9"; }
        else if(temp[i]=='!')
        { str+="0"; }
        else 
        { str+=temp[i]; }
    }
    return str;
}

string transform_back(string temp)
{
    string str="";
    int len=temp.length();
    for(int i=0;i<len;i++)
    {
        if(temp[i]>='0'&&temp[i]<='9')
            str+=template1[temp[i]-'0'];
        else
            str+=temp[i];
    }
    return str;
}

string elim_firstlayer(string temp)
{
    int len=temp.length();
    int count=0; int layer=0; int happen=0;
    if(temp[0]=='('&&temp[len-1]==')')
    {
        for(int i=0;i<len;i++)
        {
            if(temp[i]=='(') { count++; if(count==1) happen++; }
            else if(temp[i]==')') count--;
            else if(temp[i]=='1'&&count==1)
            {
                if(layer!=0) return temp;
                layer=1;
            }   
            else if(temp[i]=='2'&&count==1)
            {
                if(layer!=0) return temp;
                layer=2;
            }
        }
        if(happen==1)
        return temp.substr(1,len-2);
    }
    return temp;
}

//will tranform all p->q to !p||q , ( p3q to 0p2q )
string implication(string t)
{
    string temp=t;
    int len = temp.length();
    for(int i=0;i<len;i++)
    {
        if(temp[i]=='3')
        {
            temp[i]='2';
            int count=0;
            for(int j=i-1;j>=0;j--)
            {
                if(count==0&& ((temp[j]>='a'&&temp[j]<='z')||(temp[j]>='A'&&temp[j]<='Z')||temp[j]=='8'||temp[j]=='9') )
                {
                    temp.insert(j,"0");
                    break;
                }
                else if(temp[j]=='(')
                {
                    count--;
                    if(count==0)
                    {
                       temp.insert(j,"0");
                       break;
                    }
                }
                else if(temp[j]==')')
                {
                    count++;
                }
            }
            len++;
        }
    }
    return temp;
}

string equivalence(string temp)
{
    int len = temp.length();
    for(int i=0;i<len;i++)
    {
        if(temp[i]=='4')
        {
            string front=""; int f=0;
            string back=""; int b=0;
            int count =0;
            for(int j=i-1;j>=0;j--)
            {
                if(count==0&& ((temp[j]>='a'&&temp[j]<='z')||(temp[j]>='A'&&temp[j]<='Z')||temp[j]=='8'||temp[j]=='9') )
                {
                    if(j-1>=0&&temp[j-1]=='0') j--;
                    f = j;
                    front = temp.substr(j,i-j);
                    break;
                }
                else if(temp[j]=='(')
                {
                    count--;
                    if(count==0)
                    {
                       if(j-1>=0&&temp[j-1]=='0') j--;
                       f=j;
                       front = temp.substr(j,i-j);
                       break;
                    }
                }
                else if(temp[j]==')') count++;
            }
            count =0;
            for(int j=i+1;j<len;j++)
            {
                if(count==0&& ((temp[j]>='a'&&temp[j]<='z')||(temp[j]>='A'&&temp[j]<='Z')||temp[j]=='8'||temp[j]=='9') )
                {
                    b=j;
                    back = temp.substr(i+1,j-i);
                    break;
                }
                else if(temp[j]==')')
                {
                    count--;
                    if(count==0)
                    {
                        b=j;
                        back = temp.substr(i+1,j-i);
                        break;
                    }
                }
                else if(temp[j]=='(') count++;
            }
            string temp2= "(0"+front+"2"+back+")1(0"+back+"2"+front+")";
            temp.replace(f,b-f+1,temp2);
        }
        len = temp.length();
    }
    return temp;
}

string demorgan(string temp)
{
    int len = temp.length();
    for(int i=0;i<len;i++)
    {
        if(temp[i]=='0'&&temp[i+1]=='(')
        {
            temp.replace(i,1,"");
            len--; i++; 
            int count=1;
            for(int j=i;j<len;j++)
            {
                if(temp[j]=='(')
                {
                    count++;
                    if(count==2)
                    {
                        temp.insert(j,"0"); j++; len++;
                    }
                }
                else if(temp[j]==')') 
                {
                    count--;
                    if(count==0) { i=j; break; }
                }
                else if(temp[j]=='2'&&count==1) temp[j]='1';
                else if(temp[j]=='1'&&count==1) temp[j]='2';
                else if((temp[j]>='a'&&temp[j]<='z')||(temp[j]>='A'&&temp[j]<='Z'))
                {
                    if(count==1)
                    {
                        temp.insert(j,"0"); len++;
                        j++;
                    }
                }
            }
        }
    }
    return temp;
}

string dnegation(string temp)
{
    int len = temp.length();
    for(int i=0;i<len;i++)
    {
        if(temp[i]=='0'&&temp[i+1]=='0')
        {
            temp.replace(i,2,"");
            len--; i--;
        }
    }
    return temp;
}
//too much problem!!! 
string dist(string temp)
{
    int len= temp.length();
    for(int i=0;i<len;i++)
    {
        int count=0;
        if(temp[i]=='2') // or case
        {   
            string front; int f=0;
            string back; int b=0;
            count=0;
            for(int j=i-1;j>=0;j--)
            {
                if(count==0&& ((temp[j]>='a'&&temp[j]<='z')||(temp[j]>='A'&&temp[j]<='Z')||temp[j]=='8'||temp[j]=='9') )
                {
                    if(j-1>=0&&temp[j-1]=='0') j--;
                    f =j;
                    front = temp.substr(j,i-j);
                    break;
                }
                else if(temp[j]=='(')
                {
                    count--;
                    if(count==0)
                    {
                       if(j-1>=0&&temp[j-1]=='0') j--;
                       f=j;
                       front = temp.substr(j,i-j);
                       break;
                    }
                }
                else if(temp[j]==')') count++;
            }
            count =0;
            for(int j=i+1;j<len;j++)
            {
                if(count==0&& ((temp[j]>='a'&&temp[j]<='z')||(temp[j]>='A'&&temp[j]<='Z')||temp[j]=='8'||temp[j]=='9') )
                {
                    b=j;
                    back = temp.substr(i+1,j-i);
                    break;
                }
                else if(temp[j]==')')
                {
                    count--;
                    if(count==0)
                    {
                        b=j;
                        back = temp.substr(i+1,j-i);
                        break;
                    }
                }
                else if(temp[j]=='(') count++;
            }
           // cout << temp <<" " << front << " "<< back; system("pause");
            //---//
            if(front.length()<=2&&back.length()<=2) {}
            else 
            {
                if(back.length()<=2) { string t = back; back=front; front =t;}
                int check=0; bool c_check=false;
                for(int j=0;j<back.length();j++) 
                {
                    if(back[j]=='(') check++;
                    else if(back[j]==')') check--;
                    else if(back[j]=='1'&&check==1) c_check=true;
                }
                if(!c_check) { string t=back; back=front; front=t; }
                //cout << temp <<" " << front << " "<< back; system("pause");
                set<string> s_str;
                int c=0;
                for(int j=0;j<back.length();j++)
                {
                    if(back[j]=='1'&&c==1)
                    {
                         string f1; string b1; int c1=0;
                         for(int k=j-1;k>=0;k--)
                         {
                            if(c1==0&& ((back[k]>='a'&&back[k]<='z')||(back[k]>='A'&&back[k]<='Z')||back[k]=='8'||back[k]=='9') )
                            {
                                if(k-1>=0&&back[k-1]=='0') k--;
                                f1 = back.substr(k,j-k);
                                break;
                            }
                            else if(back[k]=='(')
                            {
                                c1++;
                                if(c1==0)
                                {
                                    if(k-1>=0&&temp[k-1]=='0') k--;
                                    f1 = back.substr(k,j-k);
                                    break;
                                }
                            }
                            else if(back[k]==')') c1--;
                        } 
                        c1=0;
                        for(int k=j+1;k<back.length();k++)
                        {
                            if(c1==0&& ((back[k]>='a'&&back[k]<='z')||(back[k]>='A'&&back[k]<='Z')||back[k]=='8'||back[k]=='9') )
                            {
                                b1 = back.substr(j+1,k-j);
                                break;
                            }
                            else if(back[k]==')')
                            {
                                c1--;
                                if(c1==0)
                                {
                                    b1 = back.substr(j+1,k-j);
                                    break;
                                }
                            }
                            else if(back[k]=='(') c1++;
                        }
                        s_str.insert(f1); s_str.insert(b1);
                    } 
                    else if(back[j]=='(') c++;
                    else if(back[j]==')') c--;
                }
                //after find the element in back
                if(!s_str.empty())
                {
                    string temp2 = "";
                    for(set<string>::iterator it=s_str.begin(); it!=s_str.end();it++)
                    {
                        string temp3 = "(" + front + "2" + *it + ")"; 
                        temp2 += temp3 +"1";
                    }
                    temp2.replace(temp2.length()-1,1,"");
                    temp.replace(f,b-f+1,temp2);
                    //cout << temp; system("pause");
                    return temp; //here
                    i=-1;
                    len = temp.length();
                    count=0;
                }
            }   
        }
        
    }
    return temp;
}
// (p||(q||r)) <=> (p||q||r)
string elim_parent(string temp)
{
    int len=temp.length();
    int count=0;
    int a_c[100]; for(int i=0;i<99;i++) a_c[i]=0;
    for(int i=0;i<len;i++)
    {
        if(temp[i]=='(')
        {
            count++;
            a_c[count]=0;
        }
        else if(temp[i]==')')
        {
            count--;
        }
        else if(temp[i]=='1')
        {
            if(1) 
            {
                a_c[count]=1;
                if(count!=0&&a_c[count-1]==1) // back
                {
                    int f=i; int b=i; 
                    int c_f=0; int c_b=0;
                    while( !(temp[f]=='('&&c_f==0) ) 
                    {  
                        if(temp[f]==')') c_f--; 
                        if(temp[f]=='(') c_f++;
                        f--;
                    }
                    while( !(temp[b]==')'&&c_b==0) ) 
                    { 
                        if(temp[b]==')') c_b--;
                        if(temp[b]=='(') c_b++;
                        b++;
                    }
                    temp.replace(f,1,"");
                    temp.replace(b-1,1,""); 
                    len-=2;
                    i=-1;
                    count =0;
                    for(int j=0;j<99;j++) a_c[j]=0;
                }
                else if(a_c[count+1]==1) // front
                {
                    int f=i; int b=i;
                    int c_b=0;
                    while( !(temp[b]==')'&&c_b==0) ) 
                    {
                        if(temp[b]==')') c_b--;
                        if(temp[b]=='(') c_b++;
                        b--;
                    }
                    f=b;
                    int c_f =-1; f--;
                    while(c_f!=0) 
                    {
                        if(temp[f]=='(') c_f++;
                        else if(temp[f]==')') c_f--;
                        f--;
                    }
                    f++;
                    temp.replace(f,1,"");
                    temp.replace(b-1,1,"");
                    len-=2;
                    i=-1;
                    count =0;
                    for(int j=0;j<99;j++) a_c[j]=0;
                }
            }
            
        }
        else if(temp[i]=='2')
        {
            if(1) 
            {
                a_c[count]=2;
                if(count!=0&&a_c[count-1]==2)
                {
                    int f=i; int b=i;
                    int c_f=0; int c_b=0;
                    while( !(temp[f]=='('&&c_f==0) ) 
                    {
                        if(temp[f]==')') c_f--; 
                        if(temp[f]=='(') c_f++;
                        f--;
                    }
                    while( !(temp[b]==')'&&c_b==0)) 
                    {
                        if(temp[b]==')') c_b--; 
                        if(temp[b]=='(') c_b++;
                        b++;
                    }
                    temp.replace(f,1,"");
                    temp.replace(b-1,1,"");
                    len-=2;
                    i=-1;
                    count=0;
                    for(int j=0;j<99;j++) a_c[j]=0;
                }
                else if(a_c[count+1]==2)
                {
                    int f=i; int b=i;
                    int c_b=0;
                    while( !(temp[b]==')'&&c_b==0)) 
                    {
                        if(temp[b]==')') c_b--; 
                        if(temp[b]=='(') c_b++;
                        b--;
                    }
                    f=b;
                    int c_f =-1; f--;
                    while(c_f!=0) 
                    {
                        if(temp[f]=='(') c_f++;
                        else if(temp[f]==')') c_f--;
                        f--;
                    }
                    f++;
                    temp.replace(f,1,"");
                    temp.replace(b-1,1,"");
                    len-=2;
                    i=-1;
                    count =0;
                    for(int j=0;j<99;j++) a_c[j]=0;
                }
            }
        }
    }
    return temp;
}
// delete this kind of things, ((p||q)) <=> (p||q) 
string elim_useless(string temp)
{
    int len=temp.length(); int count=0;
    int check_p[100]; for(int i=0;i<99;i++) check_p[i]=0;
    for(int i=0;i<len;i++)
    {
        if(temp[i]=='(')
        {
            count++;
            check_p[count]=0;
        }
        else if(temp[i]==')')
        {
            count--;
            if(check_p[count+1]==0)
            {
                int c=-1;
                int j=i;
                while(c!=0) 
                {
                    j--;
                    if(temp[j]=='(') c++;
                    if(temp[j]==')') c--;
                    
                }
                temp.replace(j,1,"");
                temp.replace(i-1,1,"");
                len=temp.length();;
                i=-1;
                count=0; 
                for(int j=0;j<99;j++) check_p[j]=0;
            }
        }    
        else if(temp[i]>='1'&&temp[i]<='4')
        {
            check_p[count]=1;
        }
    }
    return elim_firstlayer(temp);
}
// p||p <=> p
string idempotenceOr(string temp)
{
    int len =temp.length();
    for(int i=0;i<len;i++)
    {
        if(temp[i]=='(')
        {
            int j=i;
            while(temp[j]!=')') j++;
            string temp2 = temp.substr(i,j-i+1);
            set<string> s_str;
            
            for(int k=0;k<temp2.size();k++)
            {
                if( (temp2[k]>='a'&&temp2[k]<='z')||(temp2[k]>='A'&&temp2[k]<='Z') )
                {
                    if(k>0&&temp2[k-1]=='0')
                      s_str.insert(temp2.substr(k-1,2));
                    else
                      s_str.insert(temp2.substr(k,1));
                }
            }
            string temp3 = "(";
            for(set<string>::iterator it = s_str.begin();it!=s_str.end();it++)
            {
                temp3+=*it + "2";
            }
            temp3.replace(temp3.length()-1,1,")");
            temp.replace(i,j-i+1,temp3);
            i=i+temp3.length()-1;
            len = temp.length();
        }
    }
    return temp;
}
// P&&P <=> P
string idempotence(string temp)
{
    int len =temp.length();
    set<string> s_str;
    int count=0;
    string temp3;
    for(int i=0;i<len;i++)
    {
        if(temp[i]=='(') count++;
        else if(temp[i]==')') count--;
        else if(count==0&&(temp[i]=='1'||temp[i]=='2'))
        {
            if(temp[i]=='1') temp3="1"; 
            else temp3="2";
            int j=i; int f=j-1,b=j+1;
            int c_f=0,c_b=0;
            while(f>0&&temp[f]!=temp[i]) f--;
            while(b<len&&temp[b]!=temp[i]) b++;
            if(f==0) f--; 
            if(b==len) b++;
            s_str.insert(temp.substr(f+1,i-f-1)); s_str.insert(temp.substr(i+1,b-i-1));
        } 
    }
    if(s_str.size()==0) return temp;
    string temp2="";
    for(set<string>::iterator it=s_str.begin();it!=s_str.end();it++)
    {
        temp2+=*it;
        temp2+=temp3;
    }
    return temp2.substr(0,temp2.length()-1);
}
//p&&!p <=> F
string complement(string temp)
{
    int len = temp.length();
    set<string> s_str;
    string temp4=""; int count=0; string temp3; bool already=false;
    for(int i=0;i<len;i++)
    {
        if(temp[i]=='(') count++;
        else if(temp[i]==')') count--;
        else if(count==0&&temp[i]=='1') temp4="9";
        else if(count==0&&temp[i]=='2') temp4="8";
    }
    if(temp4.size()==0) return temp;
    count=0;
    for(int i=0;i<len;i++)
    {
        if(temp[i]=='(') count++;
        else if(temp[i]==')') count--;
        else if(count==0&&(temp[i]=='1'||temp[i]=='2'))
        {
            if(temp[i]=='1') temp3="1"; 
            else temp3="2";
            int j=i; int f=j-1,b=j+1;
            int c_f=0,c_b=0;
            while(f>0&&temp[f]!=temp[i]) f--;
            while(b<len&&temp[b]!=temp[i]) b++;
            if(f==0) f--; 
            if(b==len) b++;
            s_str.insert(temp.substr(f+1,i-f-1)); s_str.insert(temp.substr(i+1,b-i-1));
        }
    }
    string temp2="";
    for(set<string>::iterator it=s_str.begin();it!=s_str.end();it++)
    {
        string t=*it; string t2;
        if(t.length()==2) t2=t.substr(1,1);
        else if(t.length()==1) t2="0"+t;
        if(t.length()>2)
        {
          temp2+=*it;
          temp2+=temp3;
        }
        else
        {
            if(s_str.find(t2)!=s_str.end()) 
            {
                if(already==false)
                { temp2+=temp4;  temp2+=temp3;  already=true; }
            }
            else temp2+=t+temp3; 
        }
    }
    return temp2.substr(0,temp2.length()-1);
}
//p||!p <=> T
string complementOr(string temp)
{
    int len=temp.length();
    for(int i=0;i<len;i++)
    {
        if(temp[i]=='(')
        {
            int j=i;
            while(temp[j]!=')') j++;
            string temp2 = temp.substr(i,j-i+1);
            set<string> s_str;
            for(int k=0;k<temp2.size();k++)
            {
                if( (temp2[k]>='a'&&temp2[k]<='z')||(temp2[k]>='A'&&temp2[k]<='Z')||temp2[k]=='8'||temp2[k]=='9' )
                {
                    string t,t2;
                    if(k>0&&temp2[k-1]=='0')
                    {
                      t  = temp2.substr(k-1,2);
                      t2 = temp2.substr(k,1);
                    }
                    else
                    {
                      t = temp2.substr(k,1);
                      t2 = "0"+t;
                    }
                    if(s_str.find(t2)!=s_str.end()) 
                    {
                     s_str.erase(t2);
                     s_str.insert("8");    
                    }
                    else s_str.insert(t);
                }
            }
            string temp3 = "(";
            for(set<string>::iterator it = s_str.begin();it!=s_str.end();it++)
            {
                temp3+=*it + "2";
            }
            temp3.replace(temp3.length()-1,1,")");
            temp.replace(i,j-i+1,temp3);
            i=i+temp3.length()-1;
            len = temp.length();
        }
    }
    temp=elim_useless(temp);
    return temp;
}
//p||F <=> p , p&&T <=> p
string identity(string temp)
{
    int len = temp.length();
    for(int i=0;i<len;i++)
    {
        if(temp[i]=='8'||temp[i]=='9')
        {
            string temp2; int direction=0;
            if(i==0&&i==len-1) {}
            else if(i==0&&(temp[i+1]=='1'||temp[i+1]=='2')) { temp2=temp[i+1]; direction =1; }
            else if(i==len-1&&(temp[i-1]=='1'||temp[i-1]=='2')) { temp2=temp[i-1]; direction=-1; }
            else 
            {
                if(temp[i+1]=='1'||temp[i+1]=='2') { temp2=temp[i+1]; direction=1; }
                else if(temp[i-1]=='1'||temp[i-1]=='2'){ temp2=temp[i-1]; direction=-1;}
            }
            if(direction!=0)
            {
                if((temp2[0]=='1'&&temp[i]=='8')||(temp2[0]=='2'&&temp[i]=='9'))
                {
                    if(direction==1)
                        temp.replace(i,2,"");
                    else
                        temp.replace(i-1,2,"");
                    len = temp.length();
                    i=-1;
                }
            }
        }
    } 
    return temp;
}
//p||T <=>T , p&&F <=> F
string annihilation(string temp)
{
    int len=temp.length(); 
    for(int i=0;i<=len;i++)
    {
        if(temp[i]=='8'||temp[i]=='9')
        {
            string temp2; int direction=0;
            if(i==0&&i==len-1) {}
            else if(i==0&&(temp[i+1]=='1'||temp[i+1]=='2')) { temp2=temp[i+1]; direction =1; }
            else if(i==len-1&&(temp[i-1]=='1'||temp[i-1]=='2')) { temp2=temp[i-1]; direction=-1; }
            else 
            {
                if(temp[i+1]=='1'||temp[i+1]=='2') { temp2=temp[i+1]; direction=1; }
                else if(temp[i-1]=='1'||temp[i-1]=='2'){ temp2=temp[i-1]; direction=-1;}
            }
            if(direction!=0)
            {
                if((temp2[0]=='1'&&temp[i]=='9')||(temp2[0]=='2'&&temp[i]=='8'))
                {
                    int f=i,b=i,c_f=0,c_b=0; 
                    while(1)
                    {
                        if(f==0||(temp[f]=='('&&c_f==0)) break;
                        if(temp[f]==')') c_f--;
                        else if(temp[f]=='(') c_f++;
                        f--;
                    }
                    while(1)
                    {
                        if(b==len-1||(temp[b]==')'&&c_b==0)) break; 
                        if(temp[b]==')') c_b--;
                        else if(temp[b]=='(') c_b++;
                        b++;
                    }
                    //cout << f <<" "<< b <<" "<< temp.substr(i,1); system("pause");
                    temp.replace(f,b-f+1,temp.substr(i,1));
                    len = temp.length();
                    i=-1;
                }
            }
        }
    }
    return temp;
}
//p&&(p||q) <=> p , only apply after idempotence
string absorption(string temp)
{
    int len = temp.length();
    vector<string> v_str;
    set<string> s_str;
    int front=0;
    for(int i=0;i<len;i++)
    {
        if((i==len-1&&!v_str.empty())||temp[i]=='1')
        {
            if(i==len-1) i++;
            v_str.push_back(temp.substr(front,i-front));
            s_str.insert(temp.substr(front,i-front));
            front =i+1;
        }
        if(temp[i]=='8'||temp[i]=='9') return temp;
    }
    if(v_str.size()==0) return temp;
    
    for(int i=0;i<v_str.size();i++)
    {
        vector<string> temp_v; // collect the string of this ()
        int f=0;
        for(int j=0;j<v_str[i].length();j++)
        {
            if(v_str[i][j]=='(') f+=1; 
            else if(v_str[i][j]=='0'||(v_str[i][j]>='a'&&v_str[i][j]<='z')||(v_str[i][j]>='A'&&v_str[i][j]<='Z')||v_str[i][j]=='8'||v_str[i][j]=='9')
            {
                if(v_str[i][j]=='0') j++;
                temp_v.push_back(v_str[i].substr(f,j-f+1));
                f=j+2;
              
            }
        }
        for(int j=0;j<v_str.size();j++)
        {
            if(i!=j&&v_str[j].size()>2)
            {
                bool f=true;
                for(int k=0;k<temp_v.size();k++)
                {
                    int found = v_str[j].find(temp_v[k]);
                    if(found==string::npos) 
                    { 
                        f=false; 
                        break; 
                    }
                    else
                    {
                        if(found!=0&&v_str[j][found-1]=='0')
                        {
                            found = v_str[j].find(temp_v[k],found+1);
                            if(found==string::npos)
                            {
                                f=false;
                                break;
                            }
                        }
                    }
                }
                if(f==true)
                {
                    s_str.erase(v_str[j]);
                }
            }
        }
    }
    string temp3;
    for(set<string>::iterator it=s_str.begin();it!=s_str.end();it++)
    {
        temp3+=*it;
        temp3+="1";
    }
    return temp3.substr(0,temp3.length()-1);
}
//p&&(!p||q) <=> p&&q // always apply after absorption to avoid bug
string reduction(string temp)
{
    int len = temp.length();
    vector<string> v_str;
    set<string> s_str;
    int front=0;
    for(int i=0;i<len;i++)
    {
        if((i==len-1&&!v_str.empty())||temp[i]=='1')
        {
            if(i==len-1) i++;
            v_str.push_back(temp.substr(front,i-front));
            s_str.insert(temp.substr(front,i-front));
            front =i+1;
        }
         if(temp[i]=='8'||temp[i]=='9') return temp;
    }
    if(v_str.size()==0) return temp;
    for(int i=0;i<v_str.size();i++)
    {
        vector<string> temp_v; // collect the string of this ()
        int f=0;
        for(int j=0;j<v_str[i].length();j++)
        {
            if(v_str[i][j]=='(') f+=1; 
            else if(v_str[i][j]=='0'||(v_str[i][j]>='a'&&v_str[i][j]<='z')||(v_str[i][j]>='A'&&v_str[i][j]<='Z')||v_str[i][j]=='8'||v_str[i][j]=='9')
            {
                if(v_str[i][j]=='0') j++; 
                temp_v.push_back(v_str[i].substr(f,j-f+1));
                f=j+2;
              
            }
        }
        if(temp_v.size()==1) //only p or !p will work
        {
            for(int j=0;j<v_str.size();j++)
            {
                if(i!=j&&v_str[j].size()>2)
                {
                    string sdf; 
                    if(temp_v[0].length()==1) sdf = "0"+temp_v[0];
                    else if(temp_v[0].length()==2) sdf = temp_v[0].substr(1,1);
                    bool f=true;
                    if(v_str[j].find(sdf)==string::npos) 
                        f=false;  
                    if(f==true)
                    {
                        int found = v_str[j].find(sdf);
                        string new_s = v_str[j]; 
                        new_s = new_s.replace(found,sdf.length(),"");
                        if(new_s[found]=='2') new_s = new_s.replace(found,1,"");
                        else new_s = new_s.replace(found-1,1,""); 
                        s_str.erase(v_str[j]);
                        s_str.insert(new_s);
                    }
                }
            }
        }
    }
    string temp3;
    for(set<string>::iterator it=s_str.begin();it!=s_str.end();it++)
    {
        temp3+=*it;
        temp3+="1";
    }
    return temp3.substr(0,temp3.length()-1);
}
//(p||q) && (p||!q) <=> p
// (!p||!q) && (!p||q) <=> !p
string adjacency(string temp)
{
    int len =temp.length();
    vector<string> v_str;
    set<string> s_str;
    int front=0;
    for(int i=0;i<len;i++)
    {
        if((i==len-1&&!v_str.empty())||temp[i]=='1')
        {
            if(i==len-1) i++;
            v_str.push_back(temp.substr(front,i-front));
            s_str.insert(temp.substr(front,i-front));
            front =i+1;
        }
         if(temp[i]=='8'||temp[i]=='9') return temp;
    }
    if(v_str.size()==0) return temp;
    
    for(int i=0;i<v_str.size();i++)
    {
        vector<string> temp_v; // collect the string of this ()
        int f=0;
        for(int j=0;j<v_str[i].length();j++)
        {
            if(v_str[i][j]=='(') f+=1; 
            else if(v_str[i][j]=='0'||(v_str[i][j]>='a'&&v_str[i][j]<='z')||(v_str[i][j]>='A'&&v_str[i][j]<='Z')||v_str[i][j]=='8'||v_str[i][j]=='9')
            {
                if(v_str[i][j]=='0') j++; 
                temp_v.push_back(v_str[i].substr(f,j-f+1));
                f=j+2;
                
            }
        }
        
        if(temp_v.size()==2) //only size two
        {
            //only two cases (p||q) , p||q
            for(int j=0;j<v_str.size();j++)
            {
                if(i!=j&&v_str[j].size()>2)
                {
                    string p1,p2,q1,q2; 
                    p1=temp_v[0]; 
                    q1=temp_v[1];
                    if(p1.length()==1) p2 = "0"+p1;
                    else if(p1.length()==2) p2 = p1.substr(1,1);
                    if(q1.length()==1) q2 = "0"+q1;
                    else if(q1.length()==2) q2 = q1.substr(1,1);
                    string answer;
                    bool f=false;
                    if(v_str[j].find(p1)!=string::npos&&v_str[j].find(q2)!=string::npos)
                        if(p1.length()==1&&v_str[j].find(p2)!=string::npos) {}
                        else if(q1.length()==2&&v_str[j].find(q1)!=string::npos) {}
                        else
                          { answer = q2; f=true; }//answer=p1; 
                    if(v_str[j].find(p2)!=string::npos&&v_str[j].find(q1)!=string::npos)
                        if(q1.length()==1&&v_str[j].find(q2)!=string::npos) {}
                        else if(p1.length()==2&&v_str[j].find(p1)!=string::npos) {}
                        else
                          { answer = p2;  f=true; }//answer=q1;  
                    if(f==true)
                    {
                        s_str.erase(v_str[j]);
                        //s_str.erase(v_str[i]);
                        int found = v_str[j].find(answer);
                        string new_s = v_str[j]; 
                        new_s = new_s.replace(found,answer.length(),"");
                        if(new_s[found]=='2') new_s = new_s.replace(found,1,"");
                        else new_s = new_s.replace(found-1,1,""); 
                        s_str.insert(new_s);
                    }
                }
            }
        }
    }
    string temp3;
    for(set<string>::iterator it=s_str.begin();it!=s_str.end();it++)
    {
        temp3+=*it;
        temp3+="1";
    }
    return temp3.substr(0,temp3.length()-1);
}
vector<int> helper;
bool used[1000];
void permutation(int i, int n, vector<vector<int> > &p)
{
    if(i==n)
    {
        p.push_back(helper);
        return;
    }
    for(int j=1;j<=n;j++)
    {
        if(!used[j])
        {
            used[j]=true;
            helper[i]=j;
            permutation(i+1,n,p);
            used[j]=false;
        }
    }
}

string sorted(string temp)
{
    int len = temp.length();
    set<string> s_str;
    string connect;
    int count=0;
    int front =0;
    for(int i=0;i<len;i++)
    {
        if((i==len-1&&!s_str.empty())||(temp[i]=='1'&&count==0)||(temp[i]=='2'&&count==0))
        {
                if(temp[i]=='1') connect = "1"; 
                else if(temp[i]=='2') connect = "2";
                if(i==len-1) i++;
                s_str.insert(temp.substr(front,i-front));
                front =i+1;
        }
        if(temp[i]=='(') count++;
        else if(temp[i]==')') count--;
    }
    if(s_str.size()==0) return temp;
    string temp3;
    for(set<string>::iterator it=s_str.begin();it!=s_str.end();it++)
    {
        string answer=*it;
        if(answer[0]=='(') answer = "(" + sorted(elim_useless(*it))+ ")";
        else answer = sorted(*it);
        temp3+=answer;
        temp3+=connect;
    }
    return temp3.substr(0,temp3.length()-1);
}

//check equivalent but different variable name
bool diffvar(string str1, string str2)
{
    //sorted
    str1 = sorted(str1);
    vector<string> v_str,v_str2;
    int len = str1.length();
    for(int i=0;i<len;i++)
    {
        string new_s="";
        if((str1[i]>='a'&&str1[i]<='z')||(str1[i]>='A'&&str1[i]<='Z'))  new_s=str1[i]; 
        bool exist=false;
        for(int j=0;j<v_str.size();j++)
        {
            if(v_str[j].compare(new_s)==0) exist=true;
        }
        if(!exist&&new_s.length()!=0)
        {
            v_str.push_back(new_s);
        }
    }
    for(int i=0;i<len;i++)
    {
        string new_s;
        if((str2[i]>='a'&&str2[i]<='z')||(str2[i]>='A'&&str1[i]<='Z')) new_s=str2[i];
        bool exist=false;
        for(int j=0;j<v_str2.size();j++)
        {
            if(v_str2[j].compare(new_s)==0) exist=true;
        }
        if(!exist&&new_s.length()!=0)
        {
            v_str2.push_back(new_s);
        }
    }
    if(v_str.size()!=v_str2.size()) return false;
    //get all the permutation
    vector<vector<int> > permu; 
    if(helper.size()!=0) helper.clear(); 
    for(int i=0;i<v_str.size();i++) helper.push_back(0);
    permutation(0,v_str.size(),permu);
    for(int i=0;i<permu.size();i++)
    {
        string temp=str2;
       for(int j=0;j<permu[i].size();j++)
       {
            for(int k=0;k<temp.size();k++)
            {
                if(str2[k]==v_str2[j][0])
                {
                  temp.replace(k,1,v_str[permu[i][j]-1]);
                }
            }
       } 
       temp = sorted(temp);
       if(str1.compare(temp)==0) return true;
    }
    
    return false;
}

bool check_eq(string f1,string f2)
{
    f1 = sorted(f1);
    f2 = sorted(f2);
    if(f1.compare(f2)==0) return true;
    return false;    
}

int main()
{
    string t1,t2; 
    cout << "Input first formula: ";
    cin >> t1;
    string f1 = transform(t1);
    while(!check_wff(f1))
    {
        cout << "Not a well form formula.";
        cout << "\nPlease input a new formula: ";
        cin >> t1; 
        f1 = transform(t1); 
    }
    
    cout << "Input second formula: ";
    cin >> t2;
    string f2 = transform(t2);
    while(!check_wff(f2))
    {
        cout << "Not a well form formular.";
        cout << "\nPlease input a new formaula: ";
        cin >> t2;
        f2 = transform(t2);
    }
    //test case
    // (c||a)&&(c||c) <=> c
    // !(!a||!(!b&&(!a||b))) <=> F 
    // (!a||b)&&(a||b||d)&&!d <=> !d&&b <=> !(!d->!b)
    //1. c||!(b&&c) <=> T
    //2. (a&&!b&&c)||(a&&!b&&!c) <=> !b&&a
    //3. !(a&&b)&&(!a||b)&&(!b||b) <=> !a
    //4. (a&&b)||(b&&(b||!c))||(!b&&c) <=> b
    //5. (((a||c)&&((a&&d)||(a&&!d)))||(a&&c))||c <=> a||c
    //6. (!A&&(A||B))||((B||(A&&A))&&(A||!B)) <=> A||B
    // (A<->!B)&&(B<->(A<->C)) <=> !C&&(A<->!B)
    //cout << transform_back(dist(f1)) << "\n";
    //system("pause");
    
    vector<string> history1, history2; 
    history2.push_back(f2); history2.push_back("free space");
    string current1 =f1, current2 =f2;
    
    string f1_eu = elim_useless(current1); 
    while(f1_eu.compare(current1)!=0)
    {
        current1 = f1_eu;
        history1.push_back(current1); history1.push_back("");
        f1_eu = elim_useless(current1);
    }
    string f1_i = elim_useless(implication(current1)); 
    while(f1_i.compare(current1)!=0)
    {
        current1 =f1_i;
        history1.push_back(current1); history1.push_back("implication");
        f1_i = elim_useless(implication(current1));
    }
    //equivalence
    string f1_e = elim_useless(equivalence(current1));
    while(f1_e.compare(current1)!=0)
    {
        current1 =f1_e;
        history1.push_back(current1); history1.push_back("equivalence");
        f1_e = elim_useless(equivalence(current1)); 
    }
    //de morgan 
    string f1_de = elim_useless(demorgan(current1));
    while(f1_de.compare(current1)!=0)
    {
        current1 = f1_de;
        history1.push_back(current1); history1.push_back("de morgan");
        f1_de = elim_useless(demorgan(current1));
    } 
    string f1_dn = elim_useless(dnegation(current1));
    while(f1_dn.compare(current1)!=0)
    {
        current1 = f1_dn;
        history1.push_back(current1); history1.push_back("double negation");
        f1_dn = elim_useless(dnegation(current1));
    } 
    string f1_fl = elim_firstlayer(current1); 
    while(f1_fl.compare(current1)!=0)
    {
        current1 = f1_fl;
        history1.push_back(current1); history1.push_back("");
        f1_fl = elim_firstlayer(current1);
    } 
    
    string f1_dt = elim_useless(dist(current1)); 
    while(f1_dt.compare(current1)!=0)
    {
        current1 = f1_dt;
        history1.push_back(current1); history1.push_back("distribution");
        f1_dt = elim_useless(dist(current1));
    }
   
    string f1_ep = elim_parent(current1);  // need to repeat
    while(f1_ep.compare(current1)!=0)
    {
        current1 = f1_ep;
        history1.push_back(current1); history1.push_back("");
        f1_ep = elim_parent(current1);
    }
    //------------------------------------//
    string old_current1 = current1;
    while(1)
    {
        string f1_idr = elim_useless(idempotenceOr(current1));
        while(f1_idr.compare(current1)!=0&&f1_idr.size()!=current1.size())
        {
            current1 = f1_idr;
            history1.push_back(current1); history1.push_back("idempotence");
            f1_idr = elim_useless(idempotenceOr(current1));
        }
        string f1_id = elim_useless(idempotence(current1));
        while(f1_id.compare(current1)!=0&&f1_id.size()!=current1.size())
        {
            current1 = f1_id;
            history1.push_back(current1); history1.push_back("idempotence");
            f1_id = elim_useless(idempotence(current1));
        }
        string f1_comr = elim_useless(complementOr(current1));
        while(f1_comr.compare(current1)!=0&&f1_comr.size()!=current1.size())
        {
            current1 = f1_comr;
            history1.push_back(current1); history1.push_back("complement");
            f1_comr = elim_useless(complementOr(current1));
        }
        string f1_com = elim_useless(complement(current1));
        while(f1_com.compare(current1)!=0&&f1_com.size()!=current1.size())
        {
            current1 = f1_com;
            history1.push_back(current1); history1.push_back("complement");
            f1_com = elim_useless(complement(current1));
        }
        string f1_iden = elim_useless(identity(current1));
        while(f1_iden.compare(current1)!=0&&f1_iden.size()!=current1.size())
        {
            current1 = f1_iden;
            history1.push_back(current1); history1.push_back("identity");
            f1_iden = elim_useless(identity(current1));
        }
        string f1_anni = elim_useless(annihilation(current1));
        while(f1_anni.compare(current1)!=0&&f1_anni.size()!=current1.size())
        {
            current1 = f1_anni;
            history1.push_back(current1); history1.push_back("annihilation");
            f1_anni = elim_useless(annihilation(current1));
        }
          
        string f1_ab = elim_useless(absorption(current1));
        while(f1_ab.compare(current1)!=0&&f1_ab.size()!=current1.size())
        {
            current1 = f1_ab;
            history1.push_back(current1); history1.push_back("absoption");
            f1_ab = elim_useless(absorption(current1));
        }
        string f1_re = elim_useless(reduction(current1));
        while(f1_re.compare(current1)!=0&&f1_re.size()!=current1.size())
        {
            current1 = f1_re;
            history1.push_back(current1); history1.push_back("reduction");
            f1_re = elim_useless(reduction(current1));
        }
        string f1_adj = elim_useless(adjacency(current1));
        while(f1_adj.compare(current1)!=0&&f1_adj.size()!=current1.size())
        {
            current1 = f1_adj;
            history1.push_back(current1); history1.push_back("adjacency");
            f1_adj = elim_useless(adjacency(current1));
        }
        if(current1.compare(old_current1)==0)
        {
            break;
        }
        else old_current1=current1;
    }
    //===========================second======================================//
    string f2_eu = elim_useless(current2); 
    while(f2_eu.compare(current2)!=0)
    {
        current2 = f2_eu;
        history2.push_back(current2); history2.push_back("");
        f2_eu = elim_useless(current2);
    }
    string f2_i = elim_useless(implication(current2)); 
    while(f2_i.compare(current2)!=0)
    {
        current2 =f2_i;
        history2.push_back(current2); history2.push_back("implication");
        f2_i = elim_useless(implication(current2));
    }
    //equivalence
    string f2_e = elim_useless(equivalence(current2));
    while(f2_e.compare(current2)!=0)
    {
        current2 =f2_e;
        history2.push_back(current2); history2.push_back("equivalence");
        f2_e = elim_useless(equivalence(current2)); 
    }
    //de morgan 
    string f2_de = elim_useless(demorgan(current2));
    while(f2_de.compare(current2)!=0)
    {
        current2 = f2_de;
        history2.push_back(current2); history2.push_back("de morgan");
        f2_de = elim_useless(demorgan(current2));
    } 
    string f2_dn = elim_useless(dnegation(current2));
    while(f2_dn.compare(current2)!=0)
    {
        current2 = f2_dn;
        history2.push_back(current2); history2.push_back("double negation");
        f2_dn = elim_useless(dnegation(current2));
    } 
    string f2_fl = elim_firstlayer(current2); 
    while(f2_fl.compare(current2)!=0)
    {
        current2 = f2_fl;
        history2.push_back(current2); history2.push_back("");
        f2_fl = elim_firstlayer(current2);
    } 
    string f2_dt = elim_useless(dist(current2)); 
    while(f2_dt.compare(current2)!=0)
    {
        current2 = f2_dt;
        history2.push_back(current2); history2.push_back("distribution");
        f2_dt = elim_useless(dist(current2));
    }
   
    string f2_ep = elim_parent(current2);  // need to repeat
    while(f2_ep.compare(current2)!=0)
    {
        current2 = f2_ep;
        history2.push_back(current2); history2.push_back("");
        f2_ep = elim_parent(current2);
    }
    //------------------------------------//
    string old_current2 = current2;
    while(1)
    {
        string f2_idr = elim_useless(idempotenceOr(current2));
        while(f2_idr.compare(current2)!=0&&f2_idr.size()!=current2.size())
        {
            current2 = f2_idr;
            history2.push_back(current2); history2.push_back("idempotence");
            f2_idr = elim_useless(idempotenceOr(current2));
        }
        string f2_id = elim_useless(idempotence(current2));
        while(f2_id.compare(current2)!=0&&f2_id.size()!=current2.size())
        {
            current2 = f2_id;
            history2.push_back(current2); history2.push_back("idempotence");
            f2_id = elim_useless(idempotence(current2));
        }
        string f2_comr = elim_useless(complementOr(current2));
        while(f2_comr.compare(current2)!=0&&f2_comr.size()!=current2.size())
        {
            current2 = f2_comr;
            history2.push_back(current2); history2.push_back("complement");
            f2_comr = elim_useless(complementOr(current2));
        }
        string f2_com = elim_useless(complement(current2));
        while(f2_com.compare(current2)!=0&&f2_com.size()!=current2.size())
        {
            current2 = f2_com;
            history2.push_back(current2); history2.push_back("complement");
            f2_com = elim_useless(complement(current2));
        }
        string f2_iden = elim_useless(identity(current2));
        while(f2_iden.compare(current2)!=0&&f2_iden.size()!=current2.size())
        {
            current2 = f2_iden;
            history2.push_back(current2); history2.push_back("identity");
            f2_iden = elim_useless(identity(current2));
        }
        string f2_anni = elim_useless(annihilation(current2));
        while(f2_anni.compare(current2)!=0&&f2_anni.size()!=current2.size())
        {
            current2 = f2_anni;
            history2.push_back(current2); history2.push_back("annihilation");
            f2_anni = elim_useless(annihilation(current2));
        }
          
        string f2_ab = elim_useless(absorption(current2));
        while(f2_ab.compare(current2)!=0&&f2_ab.size()!=current2.size())
        {
            current2 = f2_ab;
            history2.push_back(current2); history2.push_back("absoption");
            f2_ab = elim_useless(absorption(current2));
        }
        string f2_re = elim_useless(reduction(current2));
        while(f2_re.compare(current2)!=0&&f2_re.size()!=current2.size())
        {
            current2 = f2_re;
            history2.push_back(current2); history2.push_back("reduction");
            f2_re = elim_useless(reduction(current2));
        }
        string f2_adj = elim_useless(adjacency(current2));
        while(f2_adj.compare(current2)!=0&&f2_adj.size()!=current2.size())
        {
            current2 = f2_adj;
            history2.push_back(current2); history2.push_back("adjacency");
            f2_adj = elim_useless(adjacency(current2));
        }
        if(current2.compare(old_current2)==0)
        {
            break;
        }
        else old_current2=current2;
    }
    string final1 = current1, final2 =current2;

    /*if(diffvar(final1,final2))
    {
        history2.push_back(final2); history2.push_back("substitution");
    }*/
    if(check_eq(final1,final2)) 
    {
        //
    }
    else 
    {
        cout << "Both formula are not equivalence.\n\n"; system("pause"); return 0; 
    } 
    cout << transform_back(f1) << "\n";
    for(int i=0;i<history1.size();i+=2)
    {
        cout << transform_back(history1[i]) << "      " << history1[i+1] << "\n";
    }
    for(int i=history2.size()-1;i>1;i-=2)
    {
        cout << transform_back(history2[i-3]) << "      " << history2[i] << "\n";
    } 
    cout << "\n"; 
    system("pause");
    return 0;
}
