public class PacketManagerRegisterInfo
{
    // 패킷 수신 및 핸들러를 등록하는 메서드입니다.
    public void Register()
    {
        _onRecv.Add((ushort)Game.PacketID.ScAttack, MakePacket<SC_ATTACK>);
        _handler.Add((ushort)Game.PacketID.ScAttack, PacketHandler.SC_Attack);

        _onRecv.Add((ushort)Game.PacketID.ScChangeWeapon, MakePacket<SC_CHANGE_WEAPON>);
        _handler.Add((ushort)Game.PacketID.ScChangeWeapon, PacketHandler.SC_ChangeWeapon);

        _onRecv.Add((ushort)Game.PacketID.ScCharacterDown, MakePacket<SC_CHARACTER_DOWN>);
        _handler.Add((ushort)Game.PacketID.ScCharacterDown, PacketHandler.SC_CharacterDown);

        _onRecv.Add((ushort)Game.PacketID.ScCharacterKillLog, MakePacket<SC_CHARACTER_KILL_LOG>);
        _handler.Add((ushort)Game.PacketID.ScCharacterKillLog, PacketHandler.SC_CharacterKillLog);

        _onRecv.Add((ushort)Game.PacketID.ScCreateMyCharacter, MakePacket<SC_CREATE_MY_CHARACTER>);
        _handler.Add((ushort)Game.PacketID.ScCreateMyCharacter, PacketHandler.SC_CreateMyCharacter);

        _onRecv.Add((ushort)Game.PacketID.ScCreateOtherCharacter, MakePacket<SC_CREATE_OTHER_CHARACTER>);
        _handler.Add((ushort)Game.PacketID.ScCreateOtherCharacter, PacketHandler.SC_CreateOtherCharacter);

        _onRecv.Add((ushort)Game.PacketID.ScGrenadeexplositionpos, MakePacket<SC_GRENADEEXPLOSITIONPOS>);
        _handler.Add((ushort)Game.PacketID.ScGrenadeexplositionpos, PacketHandler.SC_Grenadeexplositionpos);

        _onRecv.Add((ushort)Game.PacketID.ScKeyInput, MakePacket<SC_KEY_INPUT>);
        _handler.Add((ushort)Game.PacketID.ScKeyInput, PacketHandler.SC_KeyInput);

        _onRecv.Add((ushort)Game.PacketID.ScPosInterpolation, MakePacket<SC_POS_INTERPOLATION>);
        _handler.Add((ushort)Game.PacketID.ScPosInterpolation, PacketHandler.SC_PosInterpolation);

        _onRecv.Add((ushort)Game.PacketID.ScShotHit, MakePacket<SC_SHOT_HIT>);
        _handler.Add((ushort)Game.PacketID.ScShotHit, PacketHandler.SC_ShotHit);

        _onRecv.Add((ushort)Game.PacketID.ScThrowGrenade, MakePacket<SC_THROW_GRENADE>);
        _handler.Add((ushort)Game.PacketID.ScThrowGrenade, PacketHandler.SC_ThrowGrenade);
    }
}
