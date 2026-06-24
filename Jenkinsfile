pipeline {
    agent any

    triggers {
        githubPush()
    }

    stages {
        stage('Build') {
            steps {
                sh '''
                    cmake -S src -B build -DCMAKE_BUILD_TYPE=Release
                    cmake --build build
                '''
            }
        }

        stage('Test') {
            steps {
                sh './build/Patterns t Test1.xml'
            }
        }
    }

    post {
        always {
            cleanWs()
        }
    }
}
