Include("\\script\\header\\taskid.lua");
KHAITHONG120 = "H� ���c 1 th�n b� cao th�. K� n�ng 120 c�a b�n �� ���c n�ng cao."
FULLSKILL = "K� n�ng 120 c�a b�n �� ��t ��n gi�i h�n ��ng phong t�o c�c. Kh�ng luy�n th�m ���c n�a!"
TIMEPERDAY120 = "B�n luy�n c� ng�y tr�i �� th�m m�t, kh�ng c�n hi�u qu�. Mai h�y ti�p t�c!"

SKILLEXP_ARRAY=
{
	{"��i Th�a Nh� Lai Ch� ",709},
	{"��o H� Thi�n",708},
	{"B� Nguy�t Ph�t Tr�n",712},
	{"Ng� Tuy�t �n",713},
	{"M� �nh Tung",710},
	{"H�p Tinh Y�m",711},
	{"H�n Thi�n Kh� C�ng",714},
	{"Ma �m Ph� Ph�ch",715},
	{"Xu�t � B�t Nhi�m",716},
	{"L��ng Nghi Ch�n Kh� ",717}
};

function luyen120()
	local lv;
	for i=1, getn(SKILLEXP_ARRAY) do
		lv = GetMagicLevel(SKILLEXP_ARRAY[i][2]);
		if(lv > 0) then
			if(lv >= 20) then
			return 2 end
			IncSkillExp(SKILLEXP_ARRAY[i][2],10000000)
		return  1 end
	end
	return 0
end

function LastDamage(NpcIndex)
	local nValue = GetTask(TASK_RESET);
	local nLan = GetNumber(2,nValue,3);
	if(nLan >= 10) then
			Msg2Player(TIMEPERDAY120)
	return end
	
	local nRet = luyen120();
	if(nRet == 1) then
		SetTask(TASK_RESET,SetNumber(2,nValue,3,nLan+1));
		Msg2Player(KHAITHONG120);
	elseif(nRet == 2) then
		Msg2Player(FULLSKILL)
	end
	
end;

function Revive(NpcIndex)
	--set skill theo ngu hanh, ap dung cho quai cap cao (co skill)------------
	local Series = GetNpcSer(NpcIndex)
	local nSTVL, nDoc, nBang, nHoa, nLoi = 0,0,0,0,0;
	if(Series==0) then		--KIM
		SetNpcSkill(NpcIndex, 419, 50, 1);
		SetNpcSkill(NpcIndex, 424, 50, 2);
		SetNpcSkill(NpcIndex, 419, 50, 3);
		SetNpcSkill(NpcIndex, 424, 50, 4);
		nSTVL = 100;
	elseif(Series==1) then	--MOC
		SetNpcSkill(NpcIndex, 420, 50, 1);
		SetNpcSkill(NpcIndex, 425, 50, 2);
		SetNpcSkill(NpcIndex, 420, 50, 3);
		SetNpcSkill(NpcIndex, 425, 50, 4);
		nDoc = 30;--doc sat khong giong sat thuong khac, = sat thuong khac chia 5, vi no co rut' doc
	elseif(Series==2) then	--THUY
		SetNpcSkill(NpcIndex, 421, 50, 1);
		SetNpcSkill(NpcIndex, 426, 50, 2);
		SetNpcSkill(NpcIndex, 421, 50, 3);
		SetNpcSkill(NpcIndex, 426, 50, 4);
		nBang = 100; --neu quai he thuy thi cho bang sat
	elseif(Series==3) then	--HOA
		SetNpcSkill(NpcIndex, 422, 50, 1);
		SetNpcSkill(NpcIndex, 427, 50, 2);
		SetNpcSkill(NpcIndex, 422, 50, 3);
		SetNpcSkill(NpcIndex, 427, 50, 4);
		nHoa = 100;--quai he hoa thi cho hoa sat,tuy theo quai manh hay yeu ma` cho so nay, vi du 8x thi cho 75 chan han ok
	elseif(Series==4) then	--THO
		SetNpcSkill(NpcIndex, 423, 50, 1);
		SetNpcSkill(NpcIndex, 428, 50, 2);
		SetNpcSkill(NpcIndex, 423, 50, 3);
		SetNpcSkill(NpcIndex, 428, 50, 4);
		nLoi = 100;
	else					--KHONG CO HE, truong hop nay la add sai hay sao do
		SetNpcSkill(NpcIndex, 418, 50, 1);
		SetNpcSkill(NpcIndex, 418, 50, 2);
		SetNpcSkill(NpcIndex, 418, 50, 3);
		SetNpcSkill(NpcIndex, 418, 50, 4);
		nSTVL = 100;
	end
		SetNpcDmgEx(NpcIndex,nSTVL, nDoc, nBang, nHoa, nLoi,0);--sat thuong noi cong
		SetNpcDmgEx(NpcIndex,nSTVL, nDoc, nBang, nHoa, nLoi,1);--sat thuong ngoai cong
end

--Khi chet
function DeathSelf(NpcIndex)
end
