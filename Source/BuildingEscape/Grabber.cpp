// CopyRight Ryuu Sakurai 2018

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "DrawDebugHelpers.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FString objectName = GetOwner()->GetName();
	FString objectPos = GetOwner()->GetActorLocation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s is at %s"), *objectName, *objectPos);	

	physickHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();


	FindComponent();
}

void UGrabber::FindComponent()
{
	if (physickHandle)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("missing component %s: "), *(GetOwner()->GetName()));
	}

	if (inputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Found component %s: "), *(GetOwner()->GetName()));
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Relase);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("missing component %s: "), *(GetOwner()->GetName()));
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Press Grab"));
	GetFirstPhysicsBodyInReach();
}

void UGrabber::Relase()
{
	UE_LOG(LogTemp, Warning, TEXT("Relase Grab"));
}
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector playerViewPoint;
	FRotator playerRotate;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPoint,
		OUT playerRotate
	);

	FString viePointToString = playerViewPoint.ToString();
	FString rotateToString = playerRotate.ToString();

	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Postion: %s"), *playerViewPoint.ToString(), *playerRotate.ToString());

	FVector lineTraceEnd = playerViewPoint + (playerRotate.Vector() * reach);

	DrawDebugLine(
		GetWorld(),
		playerViewPoint,
		lineTraceEnd,
		FColor(255.0f, 0.0f, 0.f),
		false,
		0.f,
		0.f,
		11.f
	);

	FHitResult Hit;

	FCollisionQueryParams trace(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		playerViewPoint,
		lineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		trace
	);
	auto actorHit = Hit.GetActor();
	if (actorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *(actorHit->GetName()));
	}

	return Hit;
}
// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

