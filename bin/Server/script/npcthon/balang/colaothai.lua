Include("\\script\\header\\taskid.lua");
function main()
	local nTaskValue = GetTask(TASK_DAOTAYTUY);
	local nTask = GetNumber(1,nTaskValue,8);
	if(nTask == 0) then
	Say(11266,2,
	"Gi�p/giup",
	"Kh�ng gi�p/kogiup"
	)
	elseif(nTask == 1) then
		if(GetItemCount(0,4,182) > 0) then
		AddItemID(AddItem(0,4,183,0,0,5,0,0))	--tranhchu
		DelItem(1,0,4,182)	--tu yen ngoc boi
		SetTask(TASK_DAOTAYTUY, SetNumber(1,nTaskValue,8,2))
		AddRepute(5)
		Msg2Player("Danh v�ng b�n t�ng th�m 5 �i�m.")
		Talk(2,"",11267,11268)
		else
		Talk(1,"",11269)
		end
	else
	Talk(1,"",RANDOM(11270,11271))
	end
end;

function giup()
	SetTask(TASK_DAOTAYTUY, SetNumber(1,GetTask(TASK_DAOTAYTUY),8,1));
	Talk(1,"",11272)
	Msg2Player("Ti�p nh�n nhi�m v� t�m con trai C� l�o th�i � b�n t�u.")
end

function kogiup()
	Talk(1,"",11273)
end
