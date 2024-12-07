import org.jetbrains.kotlin.gradle.tasks.KotlinCompile

val mainClassName: String by project

plugins {
  kotlin("jvm") version "1.6.10"
  application
  id("com.diffplug.spotless") version "6.4.2"
}

group = "me.mitch"
version = "1.0-SNAPSHOT"

repositories {
  mavenCentral()
}

dependencies {
  testImplementation(kotlin("test"))
}

tasks.test {
  useJUnitPlatform()
}

tasks.withType<KotlinCompile>() {
  kotlinOptions.jvmTarget = "11"
}

application {
  mainClass.set(project.property("mainClassName")?.toString() ?: "MainKt")
}

tasks.named<JavaExec>("run") {
  standardInput = System.`in`
}

configure<com.diffplug.gradle.spotless.SpotlessExtension> {
  kotlin {
    ktlint().userData(mapOf("indent_size" to "2", "continuation_indent_size" to "2"))
  }
  kotlinGradle {
    ktlint().userData(mapOf("indent_size" to "2", "continuation_indent_size" to "2"))
  }
}
