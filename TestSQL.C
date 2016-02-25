#include <TSQLServer.h>
#include <TSQLResult.h>
#include <TSQLRow.h>

void TestSQL()
{
	TSQLServer* db = TSQLServer::Connect("sqlite://LearningAnalytics.db", "", "");
	
	printf("Server info: %s\n", db->ServerInfo());
	
	TSQLRow* row;
	TSQLResult* res;
	
//	printf("\nList all databases on server %s\n", db->GetHost());
//	res = db->GetDataBases();
//	while ((row = res->Next())) {
//		printf("%s\n", row->GetField(0));
//		delete row;
//	}
//	delete res;
	
	printf("\nList all tables in database \"LearningAnalytics\" on server %s\n", db->GetHost());
	res = db->GetTables("LearningAnalytics");
	while ((row = res->Next())) {
		printf("%s\n", row->GetField(0));
		delete row;
	}
	delete res;
}