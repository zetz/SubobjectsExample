// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkObjectContainer.h"

#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UNetworkObjectContainer::UNetworkObjectContainer()
{
	SetIsReplicatedByDefault(true);
}

void UNetworkObjectContainer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UNetworkObjectContainer, Items);
}

bool UNetworkObjectContainer::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	bWroteSomething |= Channel->ReplicateSubobjectList(Items, *Bunch, *RepFlags);

	return bWroteSomething;
}

void UNetworkObjectContainer::OnRep_Items(const TArray<UNetworkObject*>& Previous) const
{

}

UNetworkObject* UNetworkObjectContainer::CreateNetworkObject(TSubclassOf<UNetworkObject> ClassType, FDataTableRowHandle Handle)
{
	AActor* Owner = GetOwner();
	if (Owner->HasAuthority())
	{
		UNetworkObject* NewNetworkObject = NewObject<UNetworkObject>(Owner, ClassType);
		NewNetworkObject->Initialize(Handle);

		Items.Add(NewNetworkObject);

		if (OnInventoryChanged.IsBound())
		{
			OnInventoryChanged.Broadcast({ NewNetworkObject }, {});
		}

		return NewNetworkObject;
	}

	return nullptr;
}

bool UNetworkObjectContainer::DestroyNetworkObject(UNetworkObject* Instance)
{
	if (Instance)
	{
		const AActor* Owner = GetOwner();

		if (Owner->HasAuthority())
		{
			if (Items.Remove(Instance) != INDEX_NONE)
			{
				if (OnInventoryChanged.IsBound())
				{
					OnInventoryChanged.Broadcast({ }, {Instance});
				}
				return true;
			}
		}
	}
	return false;
}




