
class  EditorUIExt : public EditorUI {
public:

    static inline  EditorUI * (* EditorUI_trp)(EditorUI *);
    EditorUI * EditorUI_hk();

    static inline  void (* EditorUIDest_trp)(EditorUI *);
    void EditorUIDest_hk();


};
