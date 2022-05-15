#pragma once
 
 
#include "main.h"
 
 
class CNetworkedVariableManager
{
public:
 
    // stores all tables, and all props inside those
    CNetworkedVariableManager( void );
 
 
    // calls GetProp wrapper to get the absolute offset of the prop
    int GetOffset( const char *tableName, const char *propName );
 
 
    // calls GetProp wrapper to get prop and sets the proxy of the prop
    bool HookProp( const char *tableName, const char *propName, RecvVarProxyFn function );
 
 
private:
 
    // wrapper so we can use recursion without too much performance loss
    int GetProp( const char *tableName, const char *propName, RecvProp **prop = 0 );
 
 
    // uses recursion to return a the relative offset to the given prop and sets the prop param
    int GetProp( RecvTable *recvTable, const char *propName, RecvProp **prop = 0 );
 
 
    RecvTable *GetTable( const char *tableName );
 
 
    std::vector<RecvTable*>    m_tables;
};
 
 
extern CNetworkedVariableManager *NetworkedVariableManager; 