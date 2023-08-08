#include<iostream>
#include<cstring>
#include<string>
#include<vector>
using namespace ::std;

vector<char>str; int len;

class JsonNode {
public:
	JsonNode() {
		ifname = 0; type = 0; name = ""; val = "";
	}//type: 1值 2数组 3对象
	static int top;//总点数而不是总边数
	vector<int>to;
	bool ifname;//0no 1yes
	int type;
	string name, val;
	void add(int y) {
		to.push_back(y);
	}
};
int JsonNode::top = 0;
vector<JsonNode>node;

bool checkname(int now) {
	int i=now;bool flag = 0;
	while (i < len) {
		if (str[i] == ' '||str[i]=='\n') {
			i++;
			continue;
		}
		if (str[i] != '\"')return 0;

		i++;
		break;
	}
	while (i < len) {//暂不处理转义符
		if (str[i] == '\"') {
			i++;
			break;
		}
		i++;
	}
	while (i < len) {
		if (str[i] == ' '||str[i]=='\n') {
			i++;
			continue;
		}
		if (str[i] != ':')return 0;
		return 1;
	}
}

pair<string,int> getname(int now) {
	int i = now; string tmps="";
	while (i < len) {
		if (str[i] == '\"') {
			i++;
			break;
		}
		i++;
	}
	while (i < len) {
		if (str[i] == '\"')break;
		
		tmps += str[i];
		i++;
	}
	while (i < len) {
		if (str[i] == ':')break;
		i++;
	}
	return make_pair(tmps, i);//结束于最后一个:处
}

int gettype(int x,int now) {
	for (int i = now; i < len; i++) {
		if (str[i] == ' '||str[i]=='\n')continue;
		if (str[i] == '[')return 2;
		if (str[i] == '{')return 3;
		return 1;
	}
}

int getval(int x, int i) {
	int j;
	for (j = i; j < len; j++) {
		if (str[j] == ' '||str[j]=='\n')continue;
		if (str[j] == '\"') {//it is a string
			node[x].val += str[j];
			int k;
			for (k = j+1; k < len; k++) {//include the ""
				node[x].val += str[k];
				if (str[k] == '\"') {
					k++;
					break;
				}
			}
			for (; k < len; k++)
				if (str[k] == '}' || str[k] == ']' || str[k] == ',')
					return k-1;//end before } ] or ,
		}
		int k;
		for (k = j; k < len; k++) {
			if (str[k] == ' '||str[k]=='\n' || str[k] == ',' || str[k] == ']' || str[k] == '}')
				break;
			node[x].val += str[k];
		}
		for (; k < len; k++) {
			if (str[k] == ',' || str[k] == ']' || str[k] == '}')
				return k-1;//end befor } ] or ,
		}
	}
}
int build(int x, int now) {
	int i=now;

	node[x].ifname = checkname(now);
	if (node[x].ifname == 1) {
		pair <string, int>tmpp = getname(now);
		node[x].name = tmpp.first;
		i = tmpp.second+1;
	}

	node[x].type=gettype(x, i);//i是去除名字的开始点
	
	if (node[x].type == 1)
	return getval(x, i);

	for (; i < len; i++) {
		if (str[i] == ' '||str[i]=='n')continue;
		i++; break;
	}
	for (; i < len; i++) {
		int y = ++JsonNode::top;
		node[x].to.push_back(y);
		JsonNode tmp;
		node.push_back(tmp);
		i = build(y, i);
		int j;
		for (j = i+1;j<len;j++) {
			if (str[j] == ',') {
				i=j+1;
				break;
			}
			if (str[j] == '}' || str[j] == ']')
				return j;
		}
	}
}

void print(int x) {
	int i;
	if (node[x].ifname == 1) {
		putchar('\"');
		cout << node[x].name;
		putchar('\"'); putchar(':');
	}
	if (node[x].type == 1) {
		cout << node[x].val;
	}
	if (node[x].type == 2) {
		putchar('[');
		for (i = 0; i < node[x].to.size(); i++) {
			print(node[x].to[i]);
			if(i!=node[x].to.size()-1)
				putchar(',');
		}
		putchar(']');
	}
	if (node[x].type == 3) {
		putchar('{');
		for (i = 0; i < node[x].to.size(); i++) {
			print(node[x].to[i]);
			if (i != node[x].to.size() - 1)
				putchar(',');
		}
		putchar('}');
	}
}

int main() {
	FILE * outputfile=freopen("123.in","r",stdin);
	freopen("123.out", "w", stdout);
	if (outputfile == nullptr){
		throw runtime_error("Fail to open!");
	}
	//get str
	char ch;
	while ((ch=getchar()) != EOF)str.push_back(ch);
	len = str.size();

	//build tree
	JsonNode tmp;
	node.push_back(tmp); node.push_back(tmp); JsonNode::top = 1;
	build(1, 0);
	print(1);
	return 0;
}