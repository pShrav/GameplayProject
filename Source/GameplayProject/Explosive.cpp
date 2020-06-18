// Explosive item that deals damage to the main character or enemy upon overlap.

#include "Explosive.h"
#include "Enemy.h"
#include "Main.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

AExplosive::AExplosive()
{
	// Setting a value for damage to be dealt
	Damage = 15.f;
}

void AExplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Inherit overlap function from parent class
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// Check other actor that overlapped
	if (OtherActor)
	{
		// Cast to main and enemy characters (convert to main/enemy class to access those functions and store results)
		AMain* Main = Cast<AMain>(OtherActor);
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);

		// If the overlapping actor was the main character or the enemy, play overlap particles and sounds (if valid)
		if (Main || Enemy)
		{
			if (OverlapParticles)
			{
				// Spawn particles at explosive's location
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f), true);
			}

			if (OverlapSound)
			{
				// Play a sound on overlapping with explosive
				UGameplayStatics::PlaySound2D(this, OverlapSound);
			}

			// Apply damage to the actor that overlapped with the explosive
			UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, DamageTypeClass);

			// Destroy explosive once damage is dealt
			Destroy();
		}
	}
}

void AExplosive::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
