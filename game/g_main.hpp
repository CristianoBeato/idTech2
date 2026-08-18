
#ifndef __GAME_MAIN_HPP__
#define __GAME_MAIN_HPP__

extern void SpawnEntities ( const char *mapname, const char *entities, const char *spawnpoint );
extern void ClientThink (edict_t *ent, usercmd_t *cmd);
extern bool ClientConnect (edict_t *ent, char *userinfo);
extern void ClientUserinfoChanged (edict_t *ent, char *userinfo);
extern void ClientDisconnect (edict_t *ent);
extern void ClientBegin (edict_t *ent);
extern void ClientCommand (edict_t *ent);
extern void RunEntity (edict_t *ent);
extern void WriteGame ( const char *filename, const bool autosave );
extern void ReadGame ( const char *filename);
extern void WriteLevel ( const char *filename);
extern void ReadLevel ( const char *filename);
extern void InitGame (void);
extern void G_RunFrame (void);

#if 0
class crGameMainLocal : public crGameMain
{
public:
    crGameMainLocal( void );
    ~crGameMainLocal( void );

    virtual void InitGame (void);
    virtual void SpawnEntities ( const char *mapname, const char *entities, const char *spawnpoint);
    virtual void ClientThink ( edict_t *ent, usercmd_t *cmd );
    virtual bool ClientConnect ( edict_t *ent, char *userinfo );
    virtual void ClientUserinfoChanged ( edict_t *ent, char *userinfo );
    virtual void ClientDisconnect ( edict_t *ent );
    virtual void ClientBegin ( edict_t *ent );
    virtual void ClientCommand ( edict_t *ent );
    virtual void RunEntity ( edict_t *ent );
    virtual void WriteGame ( const char *filename, const bool autosave );
    virtual void ReadGame ( const char *filename );
    virtual void WriteLevel ( const char *filename );
    virtual void ReadLevel ( const char *filename );
    virtual void G_RunFrame ( void );

private:

};
#endif 


#endif //!__GAME_MAIN_HPP__